Actuellement, pour construire les Array, on cherche d'abord le type et on appelle le ctor avec les même arguments
  C'est cool notamment pour éviter de passer par un appel vfs si on peut se débrouiller sans

On passe dans tag_list pour certains array_type_for_ctor_args

Mda
===

Est-ce que ça serait cool d'avoir un copy-on-write ?
  => pourquoi pas, mais il faudra prévoir des opérateurs d'accès qui ne vont pas vérifier à chaque écriture qu'il y a le droit
  => il faudra dire si on a la ref du premier parent.
  => on pourrait imaginer que les objets puissent eux-même être utilisés comme référence 
  => par exemple, on pourrait utiliser un NdArray comme objet de base

Du coup, qu'est-ce qu'il y a dans Parent ?
  * inc_ref, dec_ref

Rq: si on fait un resize dans un parent, il faudrait que les enfants continuent de fonctionner...
  On pourrait imaginer que col( i ) par exemple stocke i, plutôt qu'un pointeur qui pourra bouger

  Ou alors, on dit que 
    * demander une colonne crée des références 

Pb: 
  * il faut que auto v = mda[ i ] crée un copie
  * const auto &v = mda[ i ] crée un référence
  * et auto &v = mda[ i ] crée

Rq: on voudrait faire le minimum en C++, étant donnée qu'on pourrait faire beaucoup mieux en TL.

Pour legendre, sdot, etc..., on pourrait passer uniquement par des listes de points... 
  Cependant, ça ne donnera pas directement un type python.
  Qu'est-ce que ça couterait de faire un type de tableau "interne"
    => il faudrait a minima faire un to_np_array
    => ça veut bien dire qu'il faut un type Mda

On pourrait faire un vfs_td_impl_type( ... )
  * ça pourrait éventuellement être utilisé par tous les ctors pour trouver les ctors adaptés
  * du coup, il faudrait être assez restrictifs

Comment faire les arguments variadiques ?
  * on l'avait fait avec RtArgList, mais ce n'était pas si ouf: on utilisait en fin de compte des clés Str...
  * mais en pratique, on avait besoin de ce côté dynamique
  * et on n'avait ensuite qu'une seule étape 

  * Rq: on pourrait gérer les arguments multiples dans les ArgTrait<>::ct_cast
    =>  CtValueOf... ne ferait que le stockage


Pb: on voudrait stocker des objets via leurs pointeurs
  => il faudrait dire
    * une description précise du stockage de l'objet
    * comment obtenur

L'idée de cast, c'était de dire quel est le type réel des objets qui cachent des données, et surtout, la façon dont c'est stocké
  C'est le cas des DtObjects ou des ArgList.
    Dans le cas d'un pointeur, le cast va toujours donner un pointeur
    Dans le cas de ArgList, on va remplacer les void * par des unique_ptr<cast>
      Rq: dans le format actuel, les casts sont des lignes de codes, mais on pourrait aussi faire du code inline pour être capables de faire le cast du bon type
      => du coup, on pourrait garder le Vec<void *>, mais on aurait un destructeur capable d'aller chercher les bons types
      Rq: on pourrait directement donner le type (se passer de cast_ref)
        À ce moment là, il faudrait enlever le _vfs_object_rt_data...
        Qu'est-ce qui défini un VfsDtObject ?
          On pourrait faire un héritage
        Idéalement, quand on fait un cast, il faudrait que ça soit un static_cast. Par exemple, le cast d'un Scalar donnerait un type qui hérite de Scalar.
        Pb: on ne peut pas donner de nouveau le type d'un attribut hérité

        Ou alors, on dit que ce n'est pas un cast, mais plutôt un accès aux données
          => _actual_content
          => ou on garde _cast



  Ça peut être intéressant pour les reassign ou destroy dans la mesure où ce n'est pas sur les final refs que ça doit agir en premier lieu.
  En particulier, les final_refs ne décrivent pas la fçon 

On pourrait envisager de traiter les types de références uniquement avec la référence de l'argument de fonction
  Cependant, on peut imaginer avec les listes d'arguments avoir des pointeurs ou des références où certains arguments sont owned et d'autres non.
  Prop: un booleen pour dire si c'est owned...

La proposition, c'est d'avoir pour chaque argument
  * un "cast" qui permet de trouver le type réel avec du code généré
  * une liste de final_ref pour trouver les données que l'utilisat•rice•eur va manipuler

Avec les objects VfsDt, le cast est un VfsDtWrap, qui contient la données brute. A priori le VfsDtWrap n'a pas à gérer les références finales, et on pourrait très bien se baser uniquement sur du code généré.
  Rq: avoir plusieurs arguments dans un VfsDrWrap n'est pas obligatoire.
  Pour les collections d'arguments, on pourrait très bien passer un autre wrapper.
  
Pb du destroy: on doit appliquer la méthode au cast, et pas aux final_refs
  C'était d'ailleurs pour le destroy qu'on est partis sur l'idée de faire un cast
  Prop: on fait un nom du genre "_on_cast_"

Rq de la mort: ça serait qd même bien plus standard de passer par le destructeur pour le cast
  C'est pas grave si c'est juste pour la destruction
  Le pb c'est qu'on aimerait bien l'utiliser aussi pour reassign... mais en réalité, le reassign ne devrait faire qu'un move, ce qui est bien supporté par les virtuals

Pourquoi on ne trouve pas tous les surdefs alors qu'on a vfs_to_python_function_map au complet ?
  
Si on fait
  *  Scalar + Scalar => 
  *  Scalar + Vector

Comment fonctionne la création de tableaux ?
  Actuellement, on passe par un vfs_dt_impl_type pour sortir un type et ensuite appeler le construct
    On pourrait sortir un VirtualCtType
  => on contruit un type et on appelle ensuite le construct avec un VFS_CALL_DINK


Accès à un élément
  => on pourrait faire une ref dans le type DT. Ça permettrait d'avoir toutes les opérations liées à Item
  Par contre, ça veut dire 
    * qu'il faut être capables d'intercepter la copie
    * qu'on va se trainer un pointeur + des indices dans le VfsDtObject

L'idéal serait de savoir si les fonctions ont besoin d'une valeur ou d'une référence à modifier
  Malheureusement, on se contente aujourd'hui d'appeler des fonctions C dans savoir ce qu'elles font

Prop: si on ne peut pas rentrer une valeur dans un type, on peut générer une exception.
  Prop: VFS_CALL_METHOD_WITH_EXCEPTION permet de prendre

Pb de l'extraction de sous tableau 
  => pour faire quelque chose de général, on pourrait faire une classe avec un pointeur sur l'original + un offset en octets
  Le soucis, c'est qu'il sera interdit de modifier 

Size => as a tuple

On serait tentés de faire tous les tableaux avec ArrayImpl, mais à ce moment là, il faudra faire des surdefinitions pour éviter les récursions infinies, en particulier parce qu'on a besoin de stocker la taille et 

Rq: on pourrait utiliser Vec pour tous les Array de dim 1... mais est-ce qu'on veut en faire la même chose ? 
  En particulier, est-ce qu'on pourrait faire un COW pour les Array ?
  On pourrait notamment imaginer que les Array aient la possibilité de s'auto changer
  De façon générale, on pourrait imaginer que les objets wrappé par DT contiennent en fait le type.
  Rq: ça veut dire qu'on assume que tous les appels de fonction doivent être faits de l'extérieur pour ne pas avoir de surprise lors d'un changement de type
  Du coup, le type pour le stockage ne sera pas le type DT
  Il faudra avoir un moyen de dire comment un type d'objet doit être stocké ET par ailleurs quel type il faut utiliser pour DT
  Rq: si on wrap un entier par exemple, on va se trimbaler les infos pour les types dans les appels
  Autre questions:
    * est-ce qu'on met de l'héritage, par exemple DtArray<> : Array ? Bof
    * est-ce que c'est un moyen de gérer les COW par exemple en changeant le type si nécessaire ?
    * est-ce qu'on pourrait donner les infos de ce qui est modifié dans les arguments
    * est-ce qu'on généralise à tous les objets wrappés ? On pourrait imaginer que le format soit la clé + les autres données
      Par exemple on a des VirtualCtType qui ont une clé Str. 
    * comment se passent par exemple les opérations sur les PowerDiagram ?
      => vraisemblablement, il faudrait travailler avec des VFS_CALL partout.
        A priori, c'est transparent tant qu'on travaille sur des objets fondamentaux comme les tableaux, etc...
        En gros, il faut qu'on fasse du calcul symbolique à fond, avec parallélisation automatique, etc...
      => les WeightedPointSet pourraient simplement être virtual ?? Bof: on a besoin du multiple dispatch
        Mais en même temps, une fois le type connu, on aimerait bien faire des VFS_CALL.
        Ça veut dire qu'on ne peut plus avoir l'idée qu'il y a deux mondes séparés (par 1 VFS_CALL)
    * on pourrait se fixer sur les DtObject et oublier les clés génériques.
      Rq: on peut imaginer des maps statiques string/autre => int associées à tel ou tel type
    * que fait-on pour les listes d'arguments ?
      => on pourrait faire un apply avec tuple


Qu'est ce que ça donnerait en Tl ? 

Février => get lucky, jessy j, , ils ont choisi et personnalisé, battle de voix.
  Stage plaisir
  2 
  Manu à l'écoute des enfants


VfsObject
=========

La nouvelle approche proposée consiste à faire des objets avec vfs_attrs
On pourrait faire un CRDT

VfsObject_StdInteger<...>
VfsObject_StdScalar<...>


La nouvelle approche consiste à manipuler des objets avec leur infos de type
  => on y fera des opérations élémentaires, avec retour immédiat côté utilisation (au cas où le type change)
  => on pourrait par ailleurs imaginer faire un bout du traitement en génération de code
    Par exemple, si on sait qu'un argument ne sera utilisé qu'en lecture seule, on peut passer la référence finale
      => ça serait cool pour les objets pointés par exemple
    S'il y a de l'écriture, ... il faudrait définir comment changer le type. Ça pourrait se faire avec une fonction virtuelle

  Rq: il faudrait peut être qu'on précise dans les VFS_CALL si les arguments doivent être en écriture, par exemple avec une liste d'entier 
  Rq: ça serait plus simple à conceptualiser si les VfsObjects contenaient type et pointeur pour owner et donnée, 
    le type d'owner définissant le type de pointeur, c.à.d. s'il faut faire une inc_ref ou autre
    ... mais on pourrait très bien faire un wrapper qui se charge de ça. À ce moment là, il faut juste s'assurer d'avoir la place pour deux pointeurs pour éviter les indirections


  Rq de la mort: si on fait des pointeurs vers des sous-données, ça ne sera pas un pointeur vers VfsObject<...>
    Du coup, les pointeurs iront plutôt vers des objets C++ bruts, et idéalement, il faudrait un pointeur vers les compteurs de référence (objets en COW et objets référencés).
    Rq: il faudra aussi gérer les cycles

    Prop:
      * type + données pour pointeur vers l'owner. A priori, on peut se débrouiller pour que le compteur de référence (s'il y en a un) soit stocké avec l'objet, et on peut aussi le mettre dans la structure, après les données si on veut.
      * type + pointeur vers la donnée.

  L'idée, c'est que les types décrivent les pointeurs, ce qui permet par exemple de faire des proxy

  Rq: dans tl17, les variables sont représentée avec ExprOff... c'était a priori pour utiliser des représentation de variable en mémoire.
    On pourrait quand même imaginer travailler sur des instructions qui font la séparation depuis une zone mémoire.

Que fait-on lors du cast ?
  * on pourrait récupérer à la fois le type de l'owner et le type pour les données, mais ça risque de faire du code bloat à fond
  * on pourrait imaginer qu'il y ait une option pour récupérer uniquement le type de l'owner

Rq: si on veut récupérer une sous-partie d'un tableau, on aimerait avoir des pointeurs vers strides + sizes + data... par défaut, ça oblige à faire un new pour la structure qui va prendre tous les pointeurs. Du coup, l'owner sera le nouvel objet, avec une référence vers l'owner d'origine.

Rq: si l'objectif c'est de se ramener à des fonctions fondamentales, un Str sera représenté par une zone mémoire... mais on peut aussi imaginer.
  
Pb: idéalement, il faudrait que les types puissent aussi être décrits par des expressions...
  On pourrait 

Propositions pour améliorer Vfs
  * on spécifie dans les VfsCall si les arguments sont en E/S. 
    * ça permettra de générer moins de types d'appels (on va faire un `const &`, même si on peut faire un `&&`)
    * on pourra gérer les copy-on-write
  * on met un accesseur dans chaque VfsObject. L'idée est d'avoir un get_content(), get_type(), reassign(), get_ref()
    Par exemple, si on veut modifier un item dans une liste, on va appeler set_item


    En fait, on aimerait bien éviter d'avoir un "content" explicite qu'on manipulerait directement.
    De façon fondamentale, on pourrait imaginer que les opérations reviennent à des manipulations de la mémoire.
    Si on a ces zones mémoire, on peut directement y effectuer les opérations.

On pourrait avoir une approche "de compilation":
  * les VfsWrappers sont en fait des variables. On les voit comme des zones mémoires avec un type + éventuellement un objet owner.
    Rq: en symbolique, on ne va pas directement allouer une zone mémoire même si on connait la taille.
    Mais ça pourrait être obligatoire d'avoir une "date" de création ou une date de dernier changement pour capter les changements conditionnels
    On pourrait avoir 
      * un mode "exécution totale" où les zones sont directement allouées, et on utilise directement les pointeurs
        Ce mode ne pourrait fonctionner que si host == target
      * et un mode "compilation" où on ne va allouer que les petites tailles, et où les appels de fonction sont tracés.

    En gros, il faut qu'on sache si on a besoin de rejouer.

    Rq: l'"exécution totale" ne concerne que
      * l'allocation => on peut tester les booléens à ce moment là, notamment si la taille est trop grande. Ensuite, on se retrouve avec un type symbolique ou pas
      * les pointeurs => on peut sélectionner la fonction à appeler lors du premier appel en fonction de `host == target`
      => on pourrait tester le flag "exécution totale" uniquement pendant ces fonctions.

    Rq: il faut tester si on va chercher une valeur dans un block à une date inférieure. Le cas échéant, il faut être capables de modifier la variable pour y insérer un "input" conditionnel.

    En résumé, on manipule des variables qui représentent essentiellement des zones mémoire.
      * avec des "wrappers" typés pour appeler les bonnes fonctions avec les bon wrappers

    => ça veut qd même dire qu'il va falloir ajouter un système d'interfaçage de librairies

Rq: dans une solution générique, le type des variables est stocké dans un entier, et on ne connait pas forcément la valeur en runtime.
  On pourrait cependant imaginer qui si on n'est pas capables de trouver un type en runtime, on génère plusieurs variantes.
  Il faudrait que les demandes de type à partir d'entiers utilisent des callbacks...
  À ce moment là, ça a du sens de mettre le type en dur.

Que fait-on pour l'owning ?














