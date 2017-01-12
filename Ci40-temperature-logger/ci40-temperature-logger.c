#include <letmecreate/letmecreate.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <awa/common.h>
#include <awa/client.h>

#define OPERATION_PERFORM_TIMEOUT   (1000)

//Clean exit logic
static volatile bool running = true;

static void exit_program(int __attribute__ ((unused))signo)
{
    running = false;
}

/* Definition of the Temperature IPSO object */
static void DefineTempObject(AwaClientSession * session)
{
    AwaObjectDefinition * objectDefinition = AwaObjectDefinition_New(3303, "Temperature", 0, 1);
    AwaObjectDefinition_AddResourceDefinitionAsFloat(objectDefinition, 5700, "Temp", false, AwaResourceOperations_ReadOnly, 1);

    AwaClientDefineOperation * operation = AwaClientDefineOperation_New(session);
    AwaClientDefineOperation_Add(operation, objectDefinition);
    AwaClientDefineOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientDefineOperation_Free(&operation);
}

/* Create LWM2M Instances and Resources */
static void SetInitialValues(AwaClientSession * session)
{

    AwaClientSetOperation * operation = AwaClientSetOperation_New(session);

    AwaClientSetOperation_CreateObjectInstance(operation, "/3303/0");
    AwaClientSetOperation_CreateOptionalResource(operation, "/3303/0/5700");

    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);
}

int main(void)
{
    /* Set signal handler to exit program when Ctrl+c is pressed */
    struct sigaction action = {
        .sa_handler = exit_program,
        .sa_flags = 0
    };
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);

    /* Create and initialise client session */
    AwaClientSession * session = AwaClientSession_New();

    /* Use default IPC configuration */        
    AwaClientSession_Connect(session);

    /* Set up LWM2M objects/instances/resources */
    DefineTempObject(session);
    SetInitialValues(session);

    /* Initialise i2c interface  and enable thermo3 click*/
    i2c_init();
    i2c_select_bus(MIKROBUS_1);
    thermo3_click_enable(0);

    while (running) {
        AwaClientSession_Process(session, OPERATION_PERFORM_TIMEOUT);
        
        float temperature = 0.f;

        /* Retrieve temperature from Click board */
        thermo3_click_get_temperature(&temperature);

        /* Update the Awa resource with the new temperature */
        AwaClientSetOperation * operation = AwaClientSetOperation_New(session);
        AwaClientSetOperation_AddValueAsFloat(operation, "/3303/0/5700", temperature);
        AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
        AwaClientSetOperation_Free(&operation);

        sleep(10);

    }

    AwaClientSession_Disconnect(session);
    AwaClientSession_Free(&session);
    thermo3_click_disable();
    i2c_release();

    return 0;
}