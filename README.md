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
### API 
### omp_lock_t
### omp_nest_lock_t
### Diferencias
|caracteristica|omp_lock_t|omp_nest_lock_t|
| ------------ | --------- | --------------|
|carac 1 | h | j| 
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
