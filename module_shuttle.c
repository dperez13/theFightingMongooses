/* Extern system call stub declarations */
#include <linux/proc_fs.h>    
#include <linux/module.h>   
#include <linux/kernel.h>
#include <linux/seq_file.h> 
#include <linux/delay.h>

#define PROC_NAME "terminal"

extern long (*STUB_test_newsyscall) (int test_int);
extern int (*STUB_start_shuttle) ();
extern int (*STUB_issue_request) (char passenger_type, int initial_terminal, 
    int destination_terminal);
extern int (*STUB_stop_shuttle) ();


struct mutex m;



//0: not moving, 1:moving, -1: stopped, 2: parked
int status = -1;

//Total seats used of 25.
int seats_used = 0;

//Passenger List. [0]: children, [1]: adults without luggage
//[2]: adults with luggage
// [terminaldesired][passengertype]
int passengers[5][3];

//Current location of the shuttle (or prev. loc)
int curr_loc = 2;

//Destination location of the shuttle 
int dest_loc = -1;

//Delivered Passenger List
int passengersdelivered[5][3];

//Queue List. [terminalat][terminaldesired][type]
int passengerqueue[5][5][3];


//Total number of passengers at each terminal, regardless
//of what kind of passenger 
int totaltermpassengers[5];

//0: stopped, 1: not stopped, -1: stopping
int shuttle_stop_status = 0;

struct task_struct kthrd = NULL;


void init_arrays(){
    int i,j,k;


    for(i=0; i<5;i++){
        for(j=0; j<3;j++){
            passengers[i][j]=0;
            passengersdelivered[i][j]=0;

        }

        for(j=0;j<5;j++){
            for(k=0;k<3;k++){
                passengerqueue[i][j][k] = 0;
            }
        }


    }
}


long my_test_newsyscall(int test) {
    printk("%s: Your int is %i\n", __FUNCTION__, test);
    return test;
}


int start_shuttle(){
    if(shuttle_stop_status !=0){
        return 1;
    }

    curr_loc = 2;
    status = 0;



    return 0;
} 


int issue_request(char passenger_type, int initial_terminal, 
    int destination_terminal){

    int passenger_val = 0;

    if(passenger_type != 'C' || passenger_type != 'A'
        passenger_type != 'L'){

        return 1;
    }

    if(initial_terminal >5 || initial_terminal <1){
        return 1;
    }

    if(destination_terminal > 5 || initial_terminal < 1){
        return 1;
    }
    mutex_lock(&m);

    switch(passenger_type){
        case 'C':

        passengerqueue[initial_terminal][destination_terminal][0] +=1;

        break;

        case 'A':

        passengerqueue[initial_terminal][destination_terminal][1] +=1;

        break;

        case 'L':

        passengerqueue[initial_terminal][destination_terminal][2] +=1;

        break;
    }
    mutex_unlock(&m);

    return 0;
}

int shuttle_run_thread(){
    while(shuttle_stop_status == 1){
        if(dest_loc>=0){
            curr_loc = dest_loc;
        }
        status = -1;
        dest_loc = -1;
        status = 0;
        int passengers_wanting_loc = passengers[curr_loc][0]
        if(passengers[curr_loc][0] > 0 || passengers[curr_loc][1]>0
            passengers[curr_loc][2] > 0){
            //If users want to get off at current terminal. 
            mutex_lock(&m);
            {
                passengersdelivered[curr_loc][0] += passengers[curr_loc][0];
                seats_used -= passengers[curr_loc][0]*1;
                passengers[curr_loc][0] = 0;



                passengersdelivered[curr_loc][1] += passengers[curr_loc][1];
                seats_used -= passengers[curr_loc][1]*2;
                passengers[curr_loc][1] = 0;

                passengersdelivered[curr_loc][2] += passengers[curr_loc][2];
                seats_used -= passengers[curr_loc][2]*4;
                passengers[curr_loc][2] = 0;

            }
            mutex_unlock(&m);

        }



        int passengersatterm[3];

        int k = 0;
        int j = 0;
        int i = 0;

        for(k=0; k<5; k++){
            for(j=0;j<3;j++){
                passengersatterm[j] += passengerqueue[curr_loc][k][j];
            }
            
        }

        if(passengersatterm[0] > 0 || passengersatterm[1]>0
            || passengersatterm[2]>0){

            mutex_lock(&m);
            for(k=0; k<5; k++){
                for(j=0;j<3;j++){
                    for(i=0;i<passengerqueue[curr_loc][k][j]; i++){
                        if(seats_used+ get_passenger_val(j) > 50){
                            break;
                        }
                        else{
                            passengers[k][j]++;
                            seats_used+ get_passenger_val(j);
                            passengerqueue[curr_loc][k][j]--;
                        }
                    }
                    
                }
            
            }
            mutex_unlock(&m);

        }

        dest_loc = get_next_destination();
        status = 1;
        msleep(time_to_get_somewhere(curr_loc, dest_loc));



    }
}

int get_passenger_val(int pass_type){
    if(pass_type==0){
        return 1;
    }
    else if (pass_type == 1){
        return 2;
    }
    else if (pass_type == 2){
        return 4;
    }
    else return 0;
}

int time_to_get_somewhere(int first_term, int next_term){
    return abs(next_term - first_term)* 30 *10;
}


int stop_shuttle(){
    return 0;
}

int get_next_destination(){

    int dest_loc = 0;

    if(seats_used>0){

        dest_loc = find_highest_destination();

    }
    else{
        dest_loc = find_terminal_with_most();
    }

}





int find_highest_destination(){
    int i;
    int j;
    int max_in_term=0;
    int max_term =0;
    int tot_pass;
    for(i=0; i<5;i++){
        tot_pass = 0;

        for(j=0; j<3;j++){
            if(j==0){
                tot_pass +=1 * passengers[i][j];
            }
            else if (j==1){
                tot_pass +=2 * passengers[i][j];

            }
            else if(j == 2){
                tot_pass +=4 * passengers[i][j];
            }

        }

        if(tot_pass >= max_in_term){
            max_term  = i;
            max_in_term = tot_pass;
        }
    }
    return max_term;
}



int find_terminal_with_most(){
    int i;
    int max_in_term=0;
    int max_term =0;
    for(i=0; i<5;i++){
        if(totaltermpassengers[i] >= max_in_term){
            max_term  = i;
            max_in_term = totaltermpassengers[i];
        }
    }
    return max_term;
}

int get_awl_passengers(){

    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
       totpass+=passengers[i][2];
        
    }
    return totpass

}

int get_awol_passengers(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
       totpass+=passengers[i][1];
        
    }
    return totpass
}

int get_chd_passengers(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
       totpass+=passengers[i][0];
        
    }
    return totpass
}

int get_tot_passengers(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
        for(j=0;j<3;j++){
            totpass+=passengers[i][j];
        }
    }
    return totpass
}



int get_awl_passengers_del(){

    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
        
       totpass+=passengersdelivered[i][2];
    }
    return totpass

}

int get_awol_passengers_del(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
       totpass+=passengersdelivered[i][1];
        
    }
    return totpass
}

int get_chd_passengers_del(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
       totpass+=passengersdelivered[i][0];
        
    }
    return totpass
}

int get_tot_passengers_del(){
    int i;
    int totpass = 0;
    int j;

    for(i=0;i<5;i++){
        for(j=0;j<3;j++){
            totpass+=passengersdelivered[i][j];
        }
    }
    return totpass
}



static int shuttle_seq_show(struct seq_file *s, void *v){


    if(shuttle_stop_status == -1){
        seq_printf(s, "Status: DEACTIVATING");
    }
    else if(moving == 0){
        seq_printf(s, "Status: PARKED");
    }
    else if(moving == 1){
        seq_printf(s, "Status: MOVING");
    }
    else if(moving == -1){
        seq_printf(s, "Status: OFFLINE");
    }


    //TODO: print seats used/available.
    int tot_passengers = get_tot_passengers();
    int chd_passengers = get_chd_passengers();
    int awl_passengers = get_awl_passengers();
    int aol_passengers = get_awol_passengers();

    seq_printf(s, "Passengers: %d (%d adult with luggage, %d adult without luggage, %d children)", tot_passengers, 
        chd_passengers, awl_passengers, aol_passengers);

    seq_printf(s, "Location: %d", curr_loc+1);
    seq_printf(s, "Destination: %d", dest_loc+1)

    int tot_passengersd = get_tot_passengers_del();
    int chd_passengersd = get_chd_passengers_del();
    int awl_passengersd = get_awl_passengers_del();
    int aol_passengersd = get_awol_passengers_del();

    seq_printf(s, "Delivered: %d (%d adult with luggage, %d adult without luggage, %d children)",
        tot_passengersd, chd_passengersd, awl_passengersd, aol_passengersd);


}

static int shuttle_proc_open(struct inode *inode, struct file *file){
    return single_open(file, shuttle_seq_show, NULL);
}

static const struct file_operations shuttle_proc_fops = {
  .owner = THIS_MODULE,
  .open = shuttle_proc_open,
  .read = seq_read,
  .release = single_release,
};




my_module_init() {
        STUB_test_newsyscall=my_test_newsyscall;

        STUB_stop_shuttle=stop_shuttle;
        STUB_start_shuttle =start_shuttle;
        STUB_issue_request = issue_request;

        kthrd = kthread_run(&shuttle_run_thread, NULL, "thread1");
        proc_create(PROC_NAME, 0, NULL, &shuttle_proc_fops)


        return 0;
}
my_module_exit() {
        STUB_test_newsyscall=NULL;
        STUB_stop_shuttle=NULL;
        STUB_start_shuttle =NULL;
        STUB_issue_request = NULL;
        kthread_stop(kthrd);

}
