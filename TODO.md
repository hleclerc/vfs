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


