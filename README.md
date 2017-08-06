# StepMotorFreeRtos
Prueba de FreeRtos con Motor paso a paso

Se implementa libreria semphr.h para administrar el uso de los perifericos Serial y los pines de conexion al Motor PAP

#Constructor para las banderas

```
SemaphoreHandle_t xSerialSemaphore;// declaramos la bandera comunicacion serial
SemaphoreHandle_t xMpapSemaphore;//declaramos bandera para motorPAP

```
se inicializa la banderas 


```
xNameFlag= xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
  if ( ( xNameFlag ) != NULL ){
    xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
```

Antes de utilizarce el recurso se verifica su disponibilidad del recurso.

```
 if(xSemaphoreTake(xNameFlag,( TickType_t ) 5 ) == pdTRUE){

```
luego de utilizar el recurso lo liberamos


```
	xSemaphoreGive(xNameFlag);
```