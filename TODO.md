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


Tableau bi-dimensionels
  => 



  ['g++', '-o', '/home/leclerc/.vfs_build/obj/ex.Link.exe.f961c6a8d378a483a146713a5324f3f4.0.exe', '/home/leclerc/.vfs_build/obj/ex.Compiler.ex.01d2e3f602c1d5fb622a720b91c98b07.0.o', '/home/leclerc/.vfs_build/obj/read_arg_name.Compiler.read_arg_name.8d4b05d74d77b218c2a499cd8cb6a7ed.0.o', '/home/leclerc/.vfs_build/obj/DisplayContext.Compiler.DisplayContext.c416ef7c489d30b25f3f0585e2f208cb.0.o', '/home/leclerc/.vfs_build/obj/DisplayParameters.Compiler.DisplayParameters.480243fb014bfb0278dc6bbfe11c2243.0.o', '/home/leclerc/.vfs_build/obj/DisplayItem.Compiler.DisplayItem.add1ab1dc631ad872b38d3a55ac4c6cb.0.o', '/home/leclerc/.vfs_build/obj/Displayer.Compiler.Displayer.4f58c8b256f22f22426efd9ac4e2b902.0.o', '/home/leclerc/.vfs_build/obj/display.Compiler.display.ad48d898e838512b3f9d715c06f86a4b.0.o', '/home/leclerc/.vfs_build/obj/DisplayItem_Object.Compiler.DisplayItem_Object.d5ad005d2c6dd6aa36f517c2689b71e1.0.o', '/home/leclerc/.vfs_build/obj/DisplayItem_Array.Compiler.DisplayItem_Array.813eb4203cb898b82e4bee307aa3fc22.0.o', '/home/leclerc/.vfs_build/obj/DisplayItem_String.Compiler.DisplayItem_String.256f13960e4dcde3670fc5fd3d302f31.0.o', '/home/leclerc/.vfs_build/obj/String.Compiler.String.7fc3a409ff66655a07f4140016c749e0.0.o', '/home/leclerc/.vfs_build/obj/OnInit.Compiler.OnInit.0fc3056be996485202f537fc7d434758.0.o', '/home/leclerc/.vfs_build/obj/VfsDtTypeTable.Compiler.VfsDtTypeTable.ace3c4dc31b867407a4c6ac0d5bacadb.0.o', '/home/leclerc/.vfs_build/obj/Type.Compiler.Type.3bf05d7f8821d47c0c650fe718b7f90a.0.o', '/home/leclerc/.vfs_build/obj/string_replace.Compiler.string_replace.5d53a2e806be868f7476a61e1d9a1e60.0.o', '/home/leclerc/.vfs_build/obj/CompilationFlags.Compiler.CompilationFlags.140788a3c6b8295567310522aa56d52f.0.o', '/home/leclerc/.vfs_build/obj/VirtualCtString.Compiler.VirtualCtString.a651d31e7423973be5e7b25e056fe29a.0.o', '/home/leclerc/.vfs_build/obj/va_string.Compiler.va_string.d0ea4fddc7e545ed2f0595657cc4faca.0.o', '/home/leclerc/.vfs_build/obj/get_vfs_func_inst.Compiler.get_vfs_func_inst.69a913d23626f0a7d2ed16c0560ac3e0.0.o', '/home/leclerc/.vfs_build/obj/VfsDtTypeAncestor.Compiler.VfsDtTypeAncestor.f94239fdc3c5240d02a149509de40231.0.o', '/home/leclerc/.vfs_build/obj/ctor_for.Compiler.ctor_for.5d00dee90aecacd6fc180252d5ac6746.0.o', '/home/leclerc/.vfs_build/obj/VirtualCtStringList.Compiler.VirtualCtStringList.e9cd4989a9958ec83bc767931275af2c.0.o', '/home/leclerc/.vfs_build/obj/Scalar.Compiler.Scalar.5de042ec3b27836729b46c890299061a.0.o', '/home/leclerc/.vfs_build/obj/ctor_of_string_to_string.Compiler.ctor_of_string_to_string.ff914b6e136d250eb3d21cb3a7cbf54c.0.o', '/home/leclerc/.vfs_build/obj/VfsCodegen.Compiler.VfsCodegen.b614338c8805b1dc45fa531c399eaf42.0.o', '/home/leclerc/.vfs_build/obj/generic_vfs_surdefs.Compiler.generic_vfs_surdefs.38b7e615a630b9939260f56d59200244.0.o', '/home/leclerc/.vfs_build/obj/VfsSymbolCache.Compiler.VfsSymbolCache.aea2170c32574e80b36882b4eaaefa4c.0.o', '/home/leclerc/.vfs_build/obj/used_sources.Compiler.used_sources.e71de7141598f108c876749219a658f8.0.o', '/home/leclerc/.vfs_build/obj/string_simple_match.Compiler.string_simple_match.e822460370b0081bf817791d571c57f6.0.o', '/home/leclerc/.vfs_build/obj/used_flags.Compiler.used_flags.968d5d4981baace51f9668869b8b17c2.0.o', '/home/leclerc/.vfs_build/obj/used_sources.WriteFile.92f509be48906ccdee94e451ad5a84d1.0.Compiler.used_sources.295d7506408da1e5f67a34dfba0ffce4.0.o', '/home/leclerc/.vfs_build/obj/used_flags.WriteFile.d07f4b9895aac339b575f7cb92a673de.0.Compiler.used_flags.221b1789351fbd6aad0f44c561fb43b7.0.o']
  