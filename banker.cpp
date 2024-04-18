/*
    Isaac Hraga

    Operating Systems
    Project 2: Banker's Algorithm

    4-11-2024
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class Banker{
    private:
        int numProc;      /* number of processes */ 
        int numResources; /* number of resources */ 
        int * available;  /* number of available instances of each resource */ 
        int * request;    /* amount of resources requested by specified process */
        int ** max;       /* max amount of resources a process can request, eg. 
                             max[i][j] where i = process and j=amount */
        int ** allocation;/* amount of resources allocated to a process, eg. 
                             allocation[i][j] where i = process and j=amount */
        int ** need;      /* amount of resources needed by process, eg. 
                             need[i][j] where i = process and j=amount */
        int * work;       /* used  to verify that request will work */
        bool * finish;    /* keeps track of finished processes*/
        
        
      
    public:
        Banker(int * avail, int ** m, int ** alloc, int p, int r, int * req){

            numProc = p;
            numResources = r;
            available = avail;
            max = m;
            allocation = alloc;
            request = req;

            /* initialize need matrix */
            need = new int*[numProc];
            for (int i = 0; i < numProc; i++)
                need[i] = new int[numResources];
        }

        ~Banker(){
            /* Free all allocated memory space */
            delete[] work;
            delete[] finish;

            for (int i = 0; i < numProc; ++i)
            {
                delete[] need[i];
            }
            delete[] need;
            
        }

        bool isSafe(int pid, string & sequenceOrReason){

            int procsDone = 0;
            int carry;        /* carry over value between steps 2 and 3 */

            /* check for valid request */
            for(int i = 0; i< numResources; ++i){
                if(((max[pid][i]) - (allocation[pid][i])) < request[i]){
                    sequenceOrReason = "Invalid request (exceeded max allocation)";
                    return false;
                }
            }

            /* check that banker has enough resources */
            for(int i = 0;i< numResources;++i){
                if(available[i] < request[i]){
                    sequenceOrReason = "Not enough resources for this request";
                    return false;
                }
            }
            
            /* build the need matrix */
            for(int i = 0; i< numProc; ++i){
                for(int j = 0; j< numResources; ++j){
                need[i][j] = max[i][j] - allocation[i][j];}
            }

                sequenceOrReason = "";
        
Step1:  
        
            /* instantiate the work array */
            work = new int[numResources];
            copy(available, available + numResources, work);
            
            /* test that the request will work  */
            for(int i=0; i<numResources; ++i){
                work[i] = work[i] - request[i];
                allocation[pid][i] = allocation[pid][i] + request[i];
                need[pid][i] = need[pid][i] - request[i];
            }

            /* instantiate the finished array */   
            finish = new bool[numProc];
            for(int i=0; i<numProc; ++i){
                finish[i] = false;
            }
            
            /* used to check that all resources have enough available*/
            int ok = 0; 
Step2:

        for(int i = 0; i<numProc; ++i){
            ok = 0;
            carry = i;
             /* check if finished */
            if(finish[i] == false){
                /* check if work is acceptable */
                for(int j = 0; j < numResources; ++j){
                    if(need[i][j] > work[j]){
                    --ok;
                    }
                }
            /* check that all work was handled */
                if(ok == 0) {
                    goto Step3;
                }
            }
        }


        goto Step4;

    /* adds previously allocated resouces back into work */
Step3: 
      for(int j = 0; j<numResources; ++j){
          work[j] = work[j] + allocation[carry][j];
      }

      finish[carry] = true;
      procsDone++; /* printing counter for string variation */

      /* building output */
      if(procsDone < numProc){
            sequenceOrReason.append("P" + to_string(carry) + ", ");
      } else{
            sequenceOrReason.append("P" + to_string(carry));
      }
      goto Step2;
      
Step4:      
      for(int i=0; i < numProc; ++i){
        if(finish[i] != true){
          sequenceOrReason = "This would result in an unsafe state";
          
          return false;
        }
      }
                  
      return true;

    }
};

int main (int argc, char * const argv[]){
    
    ifstream config;           /* Configuration file */
    string snapshotFile;       /* snapshot of the banker scenario */
    int numProc;               /* number of processes*/
    int numResources;          /* number of resources */
    string sequenceOrReason;   /* Return string of the Banker's Algorithm */
    int pid;                   /* ID of process making the request */

    /* Read in the snapshot file */
    if (argc < 2)
    {
        cout << "Usage: banker <snapshot file>\n";
        return 0;
    }
    else
    {
        snapshotFile = argv[1];
    }

    /* Open the file */
    config.open(snapshotFile.c_str());

    /* Declare available array, total array, max matrix, and allocation matrix */ 
    int * available ;
    int * total;
    int * request;
    int ** max;
    int ** allocation;
    

    /* used for parsing snapshot file */
    string line;

    

    /*parsing snapshot file*/
    while(config >> line){
        if(line == "Processes"){
            config >> line;
            numProc = stoi(line);
            //cout << "Proc:" << numProc << endl;
            
        }else if(line == "Resources"){
            config >> line;
            numResources = stoi(line);
            //cout << "Resources: " << numResources << "\n";

            /* Initialize available array, total array, max matrix, and allocation matrix
            with the number of processes and the number of resources */ 
            available = new int[numResources];
            total = new int[numResources];
            request = new int[numResources];
            max = new int*[numProc];
            allocation = new int*[numProc];
            for (int i = 0; i < numProc; ++i)
            {
                max[i] = new int[numResources];
                allocation[i] = new int[numResources];
            }

        }else if(line == "Total"){
            for(int i = 0; i < numResources; ++i){
                config >> line;
                total[i] = stoi(line);
                //cout << "Total: " << total[i] << "\n";
            }

        } else if(line == "Max"){
            for (int i = 0; i < numProc; ++i){
                for(int j = 0; j  < numResources; ++j){
                    config >> line;
                    max[i][j] = stoi(line);
                    //cout << "Max: " << i << ": " << max[i][j] << "\n";
                }
            }
            

        } else if(line=="Allocation"){
            for (int i = 0; i < numProc; i++)
            {
                for(int j = 0; j  < numResources; ++j){
                    config >> line;
                    allocation[i][j] = stoi(line);
                    //cout << "Allocation: " << i << ": " << allocation[i][j] << "\n";
                }
            
            }
            
        } else if(line=="Available"){
            for(int i = 0; i < numResources; ++i){
                config >> line;
                available[i] = stoi(line);
                //cout << "Available: " << available[i] << "\n";
            }
            
        } else if(line=="Process"){
                config >> line;
                pid = stoi(line);

        } else if(line=="Request"){
             for (int i = 0; i < numResources; ++i)
            {
                    config >> line;
                    request[i] = stoi(line);
                    //cout << "Request: " << i << ": " << request[i]<< "\n";
                
            
            }

        }
        if(config.eof()){
            break;
        } 
        
    }
   
    /* Initialize banker with all arrays and matrices*/
    Banker * banker = new Banker(available, max, allocation, numProc, numResources, request);

    /* Check to see if request and system is safe using Banker's Algorithm*/
    if (banker -> isSafe(pid, sequenceOrReason))
        cout << "The system is in a safe state!\n"
             << "The safe sequence is: " << sequenceOrReason << ".\n";
    else
        cout << "The system is not in a safe state.\n"
             << "Reason: " << sequenceOrReason << "\n";

    /* Free all allocated memory space */
    delete banker;
    
    for(int i = 0; i < numProc; ++i){
        delete[] max[i];
        delete[] allocation[i];
    }
    delete[] available;
    delete[] total;
    delete[] request;
    delete[] max;
    delete[] allocation;
    
     

}