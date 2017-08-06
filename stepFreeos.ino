#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Stepper.h>

SemaphoreHandle_t xSerialSemaphore;// declaramos la bandera comunicacion serial
SemaphoreHandle_t xMpapSemaphore;//declaramos bandera para motorPAP

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

void TaskMPAPtrunL( void *pvParameters );
void TaskMPAPtrunR( void *pvParameters );

boolean flag = true;

void setup() {
  Serial.begin(9600);
    myStepper.setSpeed(60); //ajustamos velocidad

  xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
  if ( ( xSerialSemaphore ) != NULL ){
    xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  xMpapSemaphore = xSemaphoreCreateMutex(); 
  if  ( ( xMpapSemaphore ) != NULL ){
    xSemaphoreGive( ( xMpapSemaphore ) );  // Make the Motor PaP available for use, by "Giving" the Semaphore.
  }
//creamos la tarea
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

}

void loop() {
  // put your main code here, to run repeatedly:

}

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
