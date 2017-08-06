# StepMotorFreeRtos
Prueba de FreeRtos con Motor paso a paso

## Semphr.h
Se implementa libreria semphr.h para administrar el uso de los perifericos Serial y los pines de conexion al Motor PAP

## Constructor para las banderas

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

## Implementacion de FreeRtos.h

Se declaran las funciones (todo se ejecuta dentro de funciones)

```
void TaskMPAPtrunL( void *pvParameters );
void TaskMPAPtrunR( void *pvParameters );
```

Creamos las tareas dentro del setup()


```
xTaskCreate(
  TaskMPAPtrunL
  ,  (const portCHAR *)"TurnLeft"  // A name just for humans
  ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL );
xTaskCreate(
  TaskMPAPtrunR
  ,  (const portCHAR *)"TurnRigth"  // A name just for humans
  ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL );
```

La funcion Loop queda vacia y acontinuacion las funciones 


```

void TaskMPAPtrunL(void *pvParameters __attribute__((unused))) {

  for(;;){
    if(xSemaphoreTake(xMpapSemaphore,( TickType_t ) 5 ) == pdTRUE){
      myStepper.step(-210);
      if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
      {
        Serial.println("Contra las manecillas");
        xSemaphoreGive( xSerialSemaphore ); 
       }
        xSemaphoreGive(xMpapSemaphore);
        vTaskDelay(1000/portTICK_PERIOD_MS);//retardo de un segundo
    }
    
  }

}
void TaskMPAPtrunR(void *pvParameters __attribute__((unused))) {
  
    for(;;){
     if(xSemaphoreTake(xMpapSemaphore,( TickType_t ) 5 ) == pdTRUE){
        myStepper.step(210);
        if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
        {
          Serial.println("Con las manecillas");
          xSemaphoreGive( xSerialSemaphore ); 
         }
          xSemaphoreGive(xMpapSemaphore);
          vTaskDelay(1000/portTICK_PERIOD_MS); 
      }else vTaskDelay(1); 
  }
}

```