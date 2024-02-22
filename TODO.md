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