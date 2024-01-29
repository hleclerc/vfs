Containers
  * on pourrait faire StdVec, SVec pour les vecteurs statiques, Vec pour les vecteurs dynamiques
    * Seq pour les vecteurs dynamiques
    * Vec pour les vecteurs 
  * sachant qu'il faudrait aussi permettre aux vecteurs d'être sur les GPUs; 
  * il faudrait aussi être capable de déterminer le type adapté d'un vecteur pour telle ou telle situation par exemple pour faire une copie
  * pour les tableaux

Est-ce qu'on pourrait gérer les arrays comme des Vec<Vec<...,SizeInParent>> ?
  * l'intérêt serait d'avoir un seul nom à gérer 
