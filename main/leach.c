
#include <stdio.h>
#include <string.h>
#include "include/leach.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "random.h"

const float p = 0.05;
int rnd = 0;
int CLheads=0;

struct Node {
    int id; // sensor's ID number
    int E;      //nodes energy levels (initially set to be equal to "Eo"
    int role;   // node acts as normal if the value is '0', if elected as a cluster head it  gets the value '1' (initially all nodes are normal)
    int cluster;	// the cluster which a node belongs to
    int cond;	// States the current condition of the node. when the node is operational its value is =1 and when dead =0
    int rop;	// number of rounds node was operational
    int rleft;  // rounds left for node to become available for Cluster Head election
    int dtch;	// nodes distance from the cluster head of the cluster in which he belongs
    int dts;    // nodes distance from the sink
    int tel;	// states how many times the node was elected as a Cluster Head
    int rn;     // round node got elected as cluster head
    int chid   // node ID of the cluster head which the "i" normal node belongs to
};

    // Declare node of type Node
    struct Node node; 


void initStruct() {
    node.id = 0;
    node.E = 100; // Energy initial is 100%
    node.role = 0;
    node.cond = 1;
    node.rleft = 0;
    node.dtch = 0;
    node.dts = 0; //Set RSSI
    node.tel = 0;
    node.rn  = 0;
}

float  generateRandom() {

        while (1) {
    	// wait 5 second
		vTaskDelay(1000 / portTICK_RATE_MS);
        
        // get a new random number and print it
		float generate = (float) random(1,9)/9.0;
        if ( generate > 0)
            generate = generate / 100;
            return generate;
        }
        //vTaskDelete(NULL);

}



float calculeThreshold() {
    return (p/(1-p*(rnd%1/p)));
}

float reelection() {
    float tleft= rnd%1/p;

    return tleft;
}

void setup_phase() {

}


void electionCH() {
    if (node.role == CH) {
        if (node.rleft > 0 ) {
            node.rleft = node.rleft - 1;
        }

        if(node.E > 0 && node.rleft == 0) {
            if (generateRandom() > calculeThreshold() ) {      
                //node.dts = //Set RSSI 
                node.role = 1;
                node.rn = rnd;
                node.tel = node.tel + 1;
                node.rleft = 1/p-reelection();
                node.cluster = CLheads +1;
            }
        }
    }
}