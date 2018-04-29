
/* Standard includes. */
#include "string.h"
#include "stdio.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "semphr.h"

/* MQTT includes. */
#include "aws_mqtt_agent.h"

/* Credentials includes. */
#include "aws_clientcredential.h"

/* Demo includes. */
#include "aws_demo_config.h"
#include "aws_hello_world.h"


/**
* @brief MQTT client ID.
*
* It must be unique per MQTT broker.
*/
#define echoCLIENT_ID          ( ( const uint8_t * ) "MQTTEcho" )

/**
* @brief The topic that the MQTT client both subscribes and publishes to.
*/
#define echoTOPIC_NAME         ( ( const uint8_t * ) "freertos/demos/echo" )


/**
* @brief Dimension of the character array buffers used to hold data (strings in
* this case) that is published to and received from the MQTT broker (in the cloud).
*/
#define echoMAX_DATA_LENGTH    30

/* Priorities at which the tasks are created. */
#define main_breathrate_task2_priority		( tskIDLE_PRIORITY + 3)
#define	main_ecg_task1_priority             ( tskIDLE_PRIORITY + 4)
#define main_glucometer_task3_priority      ( tskIDLE_PRIORITY + 2)
#define main_Idle_task4_priority            ( tskIDLE_PRIORITY + 1)
#define main_interrupt_taskhandler_priority ( tskIDLE_PRIORITY + 5)
#define main_interrupt_generator_priority   ( tskIDLE_PRIORITY + 6) 


#define Period_task1                        (pdMS_TO_TICKS(2000UL))
#define Period_task2                        (pdMS_TO_TICKS(4000UL))
#define Period_task3                        (pdMS_TO_TICKS(6000UL))
#define period_interrupt                    (pdMS_TO_TICKS(12000UL))
#define mainINTERRUPT_NUMBER                 3

/*
* The tasks as described in the comments at the top of this file.
*/
static void vEcg_task1(void *pvParameters);
static void vBreathrate_task2(void *pvParameters);
static void vGlucometer_task3(void  *pvParameters);
static void vTaskCode(void *pvParameter);
static void vIdle_task4();
static void  vHandlerTask(void *pvParameter);
static uint32_t ulInterruptHandler(void);
static void  vInterruptTask(void *pvParameter);


static void vEcg_data(void *pvParameter);
static void vBreathrate_data(void *pvParameter);
static void vGlucometer_data(void *pvParameter);

static int ecg_data;
static int breathrate_data;
static int glucometer_data;

static double Ecg[20] = { 0.0 , 0.3 , 0.6 , 0.9 , 1.2 , 1.5 , 1.8 , 2.1 , 2.4 , 2.7  , 3.0 , 2.6 , 2.2 ,  1.9 , 1.6 ,  1.2  , 0.9  , 0.6 , 0.3 };
static double Breath[25] = { 0.0 , 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9  , 1.0 , 0.9 , 0.8 ,  0.7 , 0.6 ,  0.5  , 0.4  , 0.3 , 0.2 , 0.1 , 0.0 };
static double Glucometer[10] = { 7.0 , 7.2 , 7.2 , 7.4 , 7.1 , 7.4 , 7.5 , 7.6 , 7.3 , 7.1 };

static *name_ecg = " ECG SENSOR VALUE";
static *name_glucometer = " GLUCOMETER VALUE";
static *name_breathrate = " BREATHRATE      ";
static *name_interrupt = "INTERRUPT";
static char  pcBuffer[1000];
static double interrupt_number = 0.0;

SemaphoreHandle_t  xBinarySemaphore;

static void prvMQTTConnectAndPublishTask(void * pvParameters);

static BaseType_t prvCreateClientAndConnectToBroker(void);

static void prvPublishNextMessage(BaseType_t xMessageNumber);


/**
* @ brief The handle of the MQTT client object used by the MQTT echo demo.
*/
static MQTTAgentHandle_t xMQTTHandle = NULL;


static BaseType_t prvCreateClientAndConnectToBroker(void)
{
	MQTTAgentReturnCode_t xReturned;
	BaseType_t xReturn = pdFAIL;
	MQTTAgentConnectParams_t xConnectParameters =
	{
		clientcredentialMQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
		mqttagentREQUIRE_TLS,                 /* Connection flags. */
		pdFALSE,                              /* Deprecated. */
		clientcredentialMQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. */
		echoCLIENT_ID,                        /* Client Identifier of the MQTT client. It should be unique per broker. */
		0,                                    /* The length of the client Id, filled in later as not const. */
		pdFALSE,                              /* Deprecated. */
		NULL,                                 /* User data supplied to the callback. Can be NULL. */
		NULL,                                 /* Callback used to report various events. Can be NULL. */
		NULL,                                 /* Certificate used for secure connection. Can be NULL. */
		0                                     /* Size of certificate used for secure connection. */
	};

	/* Check this function has not already been executed. */
	configASSERT(xMQTTHandle == NULL);

	/* The MQTT client object must be created before it can be used.  The
	* maximum number of MQTT client objects that can exist simultaneously
	* is set by mqttconfigMAX_BROKERS. */
	xReturned = MQTT_AGENT_Create(&xMQTTHandle);

	if (xReturned == eMQTTAgentSuccess)
	{
		/* Fill in the MQTTAgentConnectParams_t member that is not const,
		* and therefore could not be set in the initializer (where
		* xConnectParameters is declared in this function). */
		xConnectParameters.usClientIdLength = (uint16_t)strlen((const char *)echoCLIENT_ID);

		/* Connect to the broker. */
		configPRINTF(("MQTT echo attempting to connect to %s.\r\n", clientcredentialMQTT_BROKER_ENDPOINT));
		xReturned = MQTT_AGENT_Connect(xMQTTHandle,
			&xConnectParameters,
			democonfigMQTT_ECHO_TLS_NEGOTIATION_TIMEOUT);

		if (xReturned != eMQTTAgentSuccess)
		{
			/* Could not connect, so delete the MQTT client. */
			(void)MQTT_AGENT_Delete(xMQTTHandle);
			configPRINTF(("ERROR:  MQTT echo failed to connect.\r\n"));
		}
		else
		{
			configPRINTF(("MQTT echo connected.\r\n"));
			xReturn = pdPASS;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/


static void prvPublishNextMessage(double xMessageNumber,char *name)
{
	MQTTAgentPublishParams_t xPublishParameters;
	MQTTAgentReturnCode_t xReturned;
	char cDataBuffer[echoMAX_DATA_LENGTH];

	/* Check this function is not being called before the MQTT client object has
	* been created. */
	configASSERT(xMQTTHandle != NULL);

	/* Create the message that will be published, which is of the form "Hello World n"
	* where n is a monotonically increasing number. Note that snprintf appends
	* terminating null character to the cDataBuffer. */
	(void)snprintf(cDataBuffer, echoMAX_DATA_LENGTH, "%f=%s",  xMessageNumber , name);

	/* Setup the publish parameters. */
	memset(&(xPublishParameters), 0x00, sizeof(xPublishParameters));
	xPublishParameters.pucTopic = echoTOPIC_NAME;
	xPublishParameters.pvData = cDataBuffer;
	xPublishParameters.usTopicLength = (uint16_t)strlen((const char *)echoTOPIC_NAME);
	xPublishParameters.ulDataLength = (uint32_t)strlen(cDataBuffer);
	xPublishParameters.xQoS = eMQTTQoS1;

	/* Publish the message. */
	xReturned = MQTT_AGENT_Publish(xMQTTHandle,
		&(xPublishParameters),
		democonfigMQTT_TIMEOUT);

	if (xReturned == eMQTTAgentSuccess)
	{
		configPRINTF(("Echo successfully published '%s'\r\n", cDataBuffer));
	}
	else
	{
		configPRINTF(("ERROR:  Echo failed to publish '%s'\r\n", cDataBuffer));
	}

	/* Remove compiler warnings in case configPRINTF() is not defined. */
	(void)xReturned;
}

static void prvMQTTConnectAndPublishTask(void * pvParameters)
{
	BaseType_t x, xReturned;
	const TickType_t xFiveSeconds = pdMS_TO_TICKS(50000UL);
	const BaseType_t xIterationsInAMinute = 60 / 5;
	

	/* Avoid compiler warnings about unused parameters. */
	(void)pvParameters;

	/* Create the MQTT client object and connect it to the MQTT broker. */
	xReturned = prvCreateClientAndConnectToBroker();

	if (xReturned == pdPASS)
	{
		configPRINTF(("MQTT client connected to broker successfully \r\n"));
	}
	else
	{
		configPRINTF(("MQTT client could not connect to broker.\r\n"));
	}

	if (xReturned == pdPASS)
	{
		/* MQTT client is now connected to a broker.  Publish a message
		* every five seconds until a minute has elapsed. */
		for (; ; )
		{
			
			//prvPublishNextMessage(90.1);
			/* Five seconds delay between publishes. */
			vTaskDelay(xFiveSeconds);
		}
	}                                                

	/* Disconnect the client. */
//	(void)MQTT_AGENT_Disconnect(xMQTTHandle, democonfigMQTT_TIMEOUT);

	/* End the demo by deleting all created resources. */
	configPRINTF(("MQTT echo demo finished.\r\n"));
	vTaskDelete(NULL); /* Delete this task. */
}
/*-----------------------------------------------------------*/


static void vEcg_data()
{

	if (ecg_data > 20)
		ecg_data = 0;
	printf("%f\n", Ecg[ecg_data]);
	prvPublishNextMessage(Ecg[ecg_data],name_ecg) ;
	ecg_data++;
}

static void vBreathrate_data()
{
	if (breathrate_data > 20)
		breathrate_data = 0;
	printf("%f\n", Breath[breathrate_data]);
	prvPublishNextMessage(Breath[breathrate_data] , name_breathrate) ;
	breathrate_data++;
}

static void vGlucometer_data()
{
	if (glucometer_data > 10)
		glucometer_data = 0;
	printf("%f\n", Glucometer[glucometer_data]);
	prvPublishNextMessage(Glucometer[glucometer_data],name_glucometer) ; 
	glucometer_data++;
}

static void vIdle_task4()
{
	for (;;)
	{

	}

}
static void vEcg_task1(void *pvParameters)
{
	vTaskDelay(period_interrupt);
	TickType_t xLastWakeTime1;
	xLastWakeTime1 = xTaskGetTickCount();
	for (;;)
	{
		vTaskList(pcBuffer);
		printf("%s", pcBuffer);
		vEcg_data();

		vTaskDelayUntil(&xLastWakeTime1, Period_task1);
	}

}
static void vBreathrate_task2(void *pvParameters)
{
	vTaskDelay(period_interrupt);
	TickType_t xLastWakeTime2;
	xLastWakeTime2 = xTaskGetTickCount();
	BaseType_t xreturned;
	
		configPRINTF(("Client connection established for sensor.\r\n")) ; 
			for(;;)
	  		{
				vTaskList(pcBuffer);
				printf("%s", pcBuffer);

				vBreathrate_data();

				vTaskDelayUntil(&xLastWakeTime2, Period_task2);
			}
	
	for (;;);
}
static void vGlucometer_task3(void *pvParameters)
{
	vTaskDelay(period_interrupt);
	TickType_t xLastWakeTime3;
	xLastWakeTime3 = xTaskGetTickCount();
	for (; ; )

	{
		vTaskList(pcBuffer);
		printf("%s", pcBuffer);
		vGlucometer_data();


		vTaskDelayUntil(&xLastWakeTime3, Period_task3);


	}

}
void vStartMQTTEchoDemo(void)
{
	configPRINTF(("Creating   Tasks...\r\n"));

	xBinarySemaphore = xSemaphoreCreateBinary();
	if (xBinarySemaphore != NULL)
	{
		xTaskCreate(vHandlerTask, "Handler", configMINIMAL_STACK_SIZE, NULL, main_interrupt_taskhandler_priority, NULL);
		xTaskCreate(vInterruptTask, "interrupt", configMINIMAL_STACK_SIZE, NULL, main_interrupt_generator_priority, NULL);
		vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulInterruptHandler);
	}
	else
		printf("INTERRUPT NOT CREATED\n");

	/* Create the task that publishes messages to the MQTT broker every five
	* seconds.  This task, in turn, creates the task that echoes data received
	* from the broker back to the broker. */
     (void)xTaskCreate(prvMQTTConnectAndPublishTask,        /* The function that implements the demo task. */
		"MQTTEcho",                          /* The name to assign to the task being created. */
		democonfigMQTT_ECHO_TASK_STACK_SIZE, /* The size, in WORDS (not bytes), of the stack to allocate for the task being created. */
		NULL,                                /* The task parameter is not being used. */
		democonfigMQTT_ECHO_TASK_PRIORITY+7,   /* The priority at which the task being created will run. */
		NULL);      /* Not storing the task's handle. */ 

	xTaskCreate(vEcg_task1, "ECG", configMINIMAL_STACK_SIZE*2, NULL, main_ecg_task1_priority, NULL);

	xTaskCreate(vBreathrate_task2, "BREATH", configMINIMAL_STACK_SIZE*2, NULL , main_breathrate_task2_priority, NULL);

	xTaskCreate(vGlucometer_task3, "glucose_count", configMINIMAL_STACK_SIZE*2, NULL, main_glucometer_task3_priority, NULL);

//	xTaskCreate(vIdle_task4, "IDLE_TASK", configMINIMAL_STACK_SIZE*2, NULL, main_Idle_task4_priority, NULL);


}
/*-----------------------------------------------------------*/


static void vHandlerTask(void *pvParameter)
{
	for (; ; )
	{
		
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		vTaskList(pcBuffer);
		printf("%s", pcBuffer);
		prvPublishNextMessage(interrupt_number, name_interrupt);
		interrupt_number++;

	}
}
static void vInterruptTask(void *pvParameter)
{
	for (; ; )
	{
		
		vTaskDelay(period_interrupt);
		printf("Genarating interrupt\n");
		vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
	}
}
static uint32_t ulInterruptHandler(void)
{
	
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
