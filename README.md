< L’ordonnanceur LIFO contient une seule pile de tâches dont la taille maximale est donnée par le
paramètre passé à sched_init . Initialement, la pile ne contient que la tâche initiale. Lorsqu’un
thread n’a rien à faire, il dépile une tâche de la pile et l’effectue ; s’il n’y a aucune tâche prête,
le thread s’endort en attendant qu’il y en ait une. Lorsqu’une nouvelle tâche est créée, elle est empilée sur la pile.
L’ordonnanceur termine lorsque la pile est vide et tous les threads sont endormis. >

- the stack used is a chained list
- compile with : make
- execute with : ./run
- options of execution : 
  + sequential : ./run -s 
  + define number of threads (if not defined by default it will take the default number of threads of the system) : ./run -t number
  + define the size of the array that will be used : ./run -n number 
 
 #for the same example of "calculate a quicksort in parallel using multiple threads" but using multiple deques : 
 https://github.com/rafikFares/Scheduler-with-multiple-Deque-WorkStealing
