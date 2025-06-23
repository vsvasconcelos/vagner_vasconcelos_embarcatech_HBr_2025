/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Library includes. */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 1000 / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 2 )


#define LED_G 11
#define LED_B 12
#define LED_R 13

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask_1( void *pvParameters );
static void prvQueueReceiveTask_2( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters ){
bool flag = true;
TickType_t xNextWakeTime;
const unsigned long ulValueToSend_1 = 100UL;          // contsante 100 unsigned log
const unsigned long ulValueToSend_2 = 200UL;          // contsante 200 unsigned log

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

		/*
		Envio para a fila - fazendo com que a fila receba a tarefa de desbloquear e alternar o LED.
		0 (0U "zero em formato unsigned (equivalente a 0 em TickType_t)" é usado como tempo de bloco,
		para que a operação de envio não seja bloqueada - ela não precisa ser bloqueada, pois a fila
	    deve estar sempre vazia neste momento do código.
		*/
		if (flag){
		    xQueueSend( xQueue, &ulValueToSend_1, 0U );
			printf("Enviando %d \n",ulValueToSend_1);
			flag = false;

		}else if (!flag){
		    xQueueSend( xQueue, &ulValueToSend_2, 0U );
			printf("Enviando %d\n",ulValueToSend_2);
			flag = true;
		}
	}
}


static void prvQueueReceiveTask_1( void *pvParameters ){
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 100UL;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have been received from the queue, but
		is it the expected value?  If it is, toggle the LED. */
		if( ulReceivedValue == ulExpectedValue )
		{
		    printf("Tarefa 1: valor recebido %d\n", ulReceivedValue);
		    // Ciano (azul-esverdeado)
			//gpio_xor_mask(( 1u << LED_G )|( 1u << LED_B ));
			//gpio_toggle(LED_G);
			//gpio_put(LED_G, !gpio_get(LED_G));  // Equivalente a toggle
			gpio_put(LED_G, 1);
			gpio_put(LED_B, 1);
			printf("LED_G %d \n", gpio_get(LED_G));
			printf("LED_B %d \n", gpio_get(LED_B));
			printf("LED_R %d \n", gpio_get(LED_R));
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_put(LED_G, 0);
			gpio_put(LED_B, 0);
			vTaskDelay(pdMS_TO_TICKS(500)); // Toca por 500ms
			ulReceivedValue = 0U;
		}else{
		    printf("Tarefa 1 (else) - Recebido %d \n",ulReceivedValue);
		}
	}
}

static void prvQueueReceiveTask_2( void *pvParameters ){
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 200UL;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have been received from the queue, but
		is it the expected value?  If it is, toggle the LED. */
		if( ulReceivedValue == ulExpectedValue )
		{
		    printf("Tarefa 2: valor recebido %d\n", ulReceivedValue);
		    // Ciano (amarelado)
			//gpio_xor_mask(( 1u << LED_B )|( 1u << LED_R ));
			//gpio_toggle(LED_B);
			//gpio_put(LED_B, !gpio_get(LED_B));  // Equivalente a toggle
			gpio_put(LED_B, 1);
			gpio_put(LED_R, 1);
			printf("LED_G %d \n", gpio_get(LED_G));
			printf("LED_B %d \n", gpio_get(LED_B));
			printf("LED_R %d \n", gpio_get(LED_R));
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_put(LED_B, 0);
			gpio_put(LED_R, 0);
			vTaskDelay(pdMS_TO_TICKS(500)); // Toca por 500ms
			ulReceivedValue = 0U;
		}else{
		    printf("Tarefa 2 (else) - Recebido %d \n",ulReceivedValue);
		}
	}
}

int main() {
    stdio_init_all(); // Inicializa serial para printf
    // seta I/O dos leds
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    /* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

	if( xQueue != NULL ){
	    /* Start the two tasks as described in the comments at the top of this file. */
		xTaskCreate( prvQueueReceiveTask_1,				/* The function that implements the task. */
					"Rx1", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
					NULL, 								/* The parameter passed to the task - not used in this case. */
					mainQUEUE_RECEIVE_TASK_PRIORITY, 	/* The priority assigned to the task. */
					NULL );								/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvQueueReceiveTask_2,				/* The function that implements the task. */
					"Rx2", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
					NULL, 								/* The parameter passed to the task - not used in this case. */
					mainQUEUE_RECEIVE_TASK_PRIORITY, 	/* The priority assigned to the task. */
					NULL );								/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}
    // Se o scheduler retornar, algo deu errado
    for (;;) {
        // Loop infinito
    }
    return 0;
}
