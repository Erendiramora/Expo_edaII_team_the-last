# Expo_edaII_ Locks explícitos
Alumnos
- 322112364 - Cordova Villar Mario Alberto
- 322048399 - Morales Garcia Erendira Sophia
- 321206954 - Jonathan Emanuel Aguilar Campos
## Introducciòn
<!--7. Sistemas de archivos y almacenamiento
Bloquean archivos o segmentos mientras son escritos para evitar que otro proceso lea datos incompletos o inconsistentes.-->
##  Contexto y teoria
https://claude.ai/share/93749c9f-ddb1-4b70-be9b-0edcb3669374 
<!--Tipos: omp_lock_t, omp_nest_lock_t.
La API básica (omp_init_lock, omp_set_lock, omp_unset_lock, omp_destroy_lock).
Cuándo critical global se queda corto y se necesitan locks independientes.
Diferencia entre omp_lock_t y omp_nest_lock_t (riesgo de deadlock al reentrar).-->
### ¿Por qué no usar critical?

  Critical tiene un poder increible al momento de generar un bloque de código que sea estatico y mantenga a raya los hilos, pero tiene dos grandes defectos que arruinan esta magia por completo:
  
  ### Muy estricto:
  Uno de los problemas que tenemos al momento de generar un bloque critical es que estamos condicionados a que todo se tenga   que trabajar en un mismo lugar, es decir, no podemos tener dos secciones CRITICAL simultaneas (Lo vuelve ineficiente en      trabajos grandes), y al estar condicionado a un bloque entre llaves {....} estrictamente, impidiendo tratar de manera        flexible el programa
      
  ### Problema de recursividad:
  Si existe una función recursiva y dentro de ella tiene un bloque critical, entrara por primera vez de manera adecuada, sin   embargo, al moment0 de querer realizar la segunda iteracion, el hilo volvera a tratar de entrar al bloque critical, al       estar ya ocupadon (por si mismo), el programa muere en un deadlock.
      
### API 
  ### omp_init_lock_ y omp_init_nest_lock (inicialización):

  Esta función marca el inicio del candado, construyen la estrucutura de omp (prepara el espacio en memoria RAM).

  en omp_init_lock, Toma el espacio de memoria apuntado por lock (que antes tenía basura), limpia los residuos de datos y      establece su estado interno como 0 (Desbloqueado/Libre). También inicializa una cola de hilos (wait queue) inicialmente      vacía. Si un hilo intenta usar el candado antes de esta función, el programa lanzará un error de segmentación                (Segmentation Fault) porque la dirección de memoria apunta a datos corruptos.

  en omp_init_nest_lock, se sigue la misma lógica anterior, añadiendo 2 caracteristicas primordiales:
    -El identificador del hilo dueño se establece en un valor nulo/vacío.
    -El contador de anidamiento se establece explícitamente en 0.

  ### omp_set_lock y omp_set_nest_lock

  Tanto en omp_set_lock_ y omp_set_nest_lock se realiza una inspección del candado, si este esta desbloqueado (0), lo cambia   automaticamente a ocupado (1), y continua con el proceso de forma normal (cambia a un estado de suspended-bloqued).

  Sin embargo omp_set_nest_lock tiene diferencias, al momento de la validación del candado, estas son.

  1. Si el candado está libre (Contador = 0): El hilo toma el candado, el sistema registra su ID de hilo como "Dueño" y el        contador sube a 1.

  2. Si el candado está ocupado, pero el dueño es EL MISMO hilo: OpenMP lo deja pasar sin detenerlo e incrementa el contador     interno (contador++).

  3. Si el candado está ocupado por OTRO hilo: El hilo actual se congela en la cola de espera exactamente igual que en el         candado simple.

  ### omp_unset_lock y omp_unset_nest_lock

  Para el caso de omp_unset_lock esta función hace lo contrario a set, desbloquea el candado pasando de estar activo o         bloqueado (1), a un estado de desbloqueo o disponible, enviando una señal a todos los hilos que estaban en cola (wake-up-    signal), tomando al primer hilo en la fila y ejecutando nuevamente un set. Una nota importante es que solo el hilo que       ejecuta set puedese el que ejecute unset.

  para el caso de omp_unset_nest_lock se realiza la misma lógica de liberación del candado, pero en este caso, se tendra que   ir restando 1 a uno al contador, si este sigue siendo mayor que uno, se continua hasta que llegue a 0, liberando así al      candado, y volviendo a repetir el set con el primer hilo en la fila.

  ### omp_destroy_lock y omp_destroy_nest_lock

  Para ambos casos esta función aplica un free, es decir, libera el espacio de memoria que se utilizo tanto en la cola de      hilos y como en las variables utilizadas.

  NOTA: Solo se puden liberar aquellos candados que esten libres, de lo contrario pude dañar el programa.

### omp_lock_t

Esta variable define un mecanismo de sincronización binario y plano. Su función principal es actuar como un interruptor      estricto para proteger un recurso compartido, asegurando que un solo hilo pueda acceder a él a la vez.

Estructura en Memoria: Es una estructura ligera y primitiva. Solo reserva espacio en la memoria RAM para almacenar un estado binario (0 para Desbloqueado, 1 para Ocupado) y los punteros necesarios para gestionar la cola de hilos en espera (wait queue).

Comportamiento Crítico: No tiene memoria de identidad. El candado sabe si está abierto o cerrado, pero no registra qué hilo lo cerró. Por ello, si el hilo dueño intenta ejecutar un segundo bloqueo sobre él, el candado lo tratará como a un extraño y lo congelará en la cola de espera, provocando un self-deadlock de forma inmediata.

  
### omp_nest_lock_t

Esta variable define un mecanismo de sincronización avanzado y modular. Su función principal es permitir la reentrancia, es decir, dar la flexibilidad de que un mismo hilo pueda asegurar un recurso y volver a ingresar a zonas protegidas por el mismo candado sin interrumpir su propio flujo.

Estructura en Memoria: Es una estructura de datos más compleja y pesada en la RAM. Además de la cola de hilos en espera, OpenMP reserva espacio dedicado para almacenar permanentemente dos datos clave: el identificador numérico del hilo dueño (Thread ID) y un contador de anidamiento (un entero de 32 o 64 bits).

Comportamiento Crítico: Posee memoria de identidad. Cada vez que un hilo intenta adquirirlo, el candado valida quién está tocando la puerta. Si el solicitante coincide con el ID del dueño actual, el candado le otorga el acceso libremente y solo incrementa su contador interno (contador++). La cerradura se abrirá para otros hilos únicamente cuando el dueño ejecute la misma cantidad de desbloqueos y el contador regrese a 0.

### Diferencias


En omp_lock

1. El metodo unset, libera el candado y espera nuevamente otro hilo para otro set

2. El metodo set simplemente bloquea el candado, su problema principal, caer en dead lock por recursividad

3. Valida unicamente que el candado este abierto o cerrado

En omp_nest_lock

1. El metodo unset solamente soltara o liberara el candado hasta que el contador sea igual a 0.

2. El metodo set además de bloquear el candado, valida el id del hilo, y en caso de recursividad, impide el deadlock

3. Además de validar el abierto o cerrado, valida el id del hilo.


## 🔗 Presentación:
https://canva.link/0b1230f7sty6oc0
## Resultados
<!--profundizar mas en esta parte-->
![resultados locks](Imagenes/resultados_locks.png)
![resultados nest lock](Imagenes/resultados_nest_locks.png)


El uso de omp_lock_t y omp_nest_lock_t demostró ser crucial para evitar condiciones de carrera y proteger los datos compartidos. Mientras que omp_lock_t controló con éxito las operaciones de saldo mediante exclusión mutua estricta, omp_nest_lock_t resolvió el problema de las llamadas anidadas, permitiendo que un hilo reingrese al mismo candado sin bloquearse a sí mismo. Gracias a estas herramientas, el saldo final de la cuenta permaneció intacto y sin errores, lo que evidencia que una sincronización adecuada es indispensable al programar en paralelo.

## Conclusiones 
<!--cuándo conviene usarlo, para que es mas util, y en que cosas no es tan eficiente-->

## Referencias

1. El Libro Oficial de los Creadores de OpenMP
Chapman, B., Josa, G., & van der Pas, R. (2008). Using OpenMP: Portable shared memory parallel programming. MIT Press.

2. La Especificación Técnica Oficial (Documentación Base)
OpenMP Architecture Review Board. (2020). OpenMP application programming interface version 5.1. OpenMP.org. https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5-1.pdf

3. Literatura Fundamental de Computación en Paralelo
Pacheco, P. S. (2011). An introduction to parallel programming. Morgan Kaufmann.

4. Enfoque Práctico en Arquitectura de Software y Deadlocks
Williams, A. (2019). C++ concurrency in action (2.ª ed.). Manning Publications.
(Nota: Este libro es la biblia de la reentrancia y los candados mutuos a bajo nivel).

5. Artículo de Investigación sobre Rendimiento de Sincronización
Jin, H., Cao, J., & Yan, Y. (2018). Performance evaluation of synchronization mechanisms in OpenMP. International Journal of Parallel Programming, 46(4), 721-743. https://doi.org/10.1007/s10766-017-0514-x
