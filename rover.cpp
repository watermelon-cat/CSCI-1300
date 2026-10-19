#include<iostream>
#include<string>
using namespace std;


bool inbounds(int xpos, int ypos, char comm, string direction){
    if (xpos == 0 && direction == "West" && comm == 'F'){
        return false;
    }
    else if (xpos == 9 && direction == "East" && comm == 'F'){
        return false;
    }
    else if (ypos == 0 && direction == "South" && comm == 'F'){
        return false;
    }
    else if (ypos == 9 && direction == "North" && comm == 'F'){
        return false;
    }
    else{
        return true;
    }
}


void Forward(int &xpos, int &ypos, string direction){
    if (direction == "North"){
        ypos++;
    }
    else if (direction == "South"){
        ypos--;
    }
    else if (direction == "East"){
        xpos++;
    }
    else if (direction == "West"){
        xpos--;
    }

}
void Rotate(char comm, string &direction){
    string dir[4] = {"North", "East", "South", "West"};
    int current_index = 0;
    for (int i = 0; i < 4; i++) {
        if (dir[i] == direction){
            current_index = i;
            break;
        }
    }

    if (comm == 'R'){
        direction = dir[((current_index + 1) % 4)];
    }
    else if (comm == 'L'){
         direction = dir[((current_index - 1 + 4) % 4)];
    }
}

void command_success(char comm, int xpos, int ypos, string direction, int battery, int &success_comms){
    cout << "Command " << comm << " succeeded. Current coordinates: " 
        << "(" << xpos << "," << ypos << ") Heading: " << direction 
        << endl << "Battery: " << battery << endl;
    success_comms++;
}

void mission_complete_message(int xpos, int ypos, int goalx, int goaly, int battery, int success_comms, string direction){
    cout << "Mission Complete!" << endl;
    cout << "Rover ended at coordinates (" << xpos << "," << ypos << ") with heading " 
    << direction << " and battery " << battery << "." << endl;

    cout << "Number of Successful Commands: " << success_comms << endl;
    
    if (xpos == goalx && ypos == goaly){
        cout << "Goal reached and rover ";
    }
    else{
        cout << "Goal not reached and rover ";
    }
    if (battery == 40){
        cout <<"reset." << endl;
    }
    else{
        cout <<"not reset." << endl;
    }
}


void command(string input_mission, int &xpos, int &ypos, string &direction, int &battery, int &success_comms){
    char comm;
    for(int i = 0; i < input_mission.length(); i++){
        comm = input_mission[i];
        switch (comm){
            case 'F':
                if (inbounds(xpos, ypos, comm, direction) == true){
                    if (battery >= 2){
                        Forward(xpos, ypos, direction);
                        battery = battery - 2;
                        command_success(comm, xpos, ypos, direction, battery, success_comms);
                    }
                    else{
                        cout << "Command F rejected: insufficient battery" << endl;
                    }
                }
                else{
                    cout << "Command F rejected: out of bounds" << endl;
                }
                break;

            case 'L':
                if (battery >= 1){
                    Rotate(comm, direction);
                    battery--;
                    command_success(comm, xpos, ypos, direction, battery, success_comms);
                }
                else{
                    cout << "Command L rejected: insufficient battery" << endl;
                }
                break;

            case 'R':
                if (battery >= 1){
                    Rotate(comm, direction);
                    battery--;
                    command_success(comm, xpos, ypos, direction, battery, success_comms);

                }
                else{
                    cout << "Command R rejected: insufficient battery" << endl;
                }
                break;
            case 'S':
                if (battery >= 3){
                    battery--;
                    command_success(comm, xpos, ypos, direction, battery, success_comms);

                }
                else{
                    cout << "Command S rejected: insufficient battery" << endl;
                }
                break;
            case 'C':
                if (xpos == 0 && ypos == 0){
                    battery = 40;
                    command_success(comm, xpos, ypos, direction, battery, success_comms);
                }
                else{
                    cout << "Command C rejected: not on charging station" << endl;
                }
                break;
            
            default:
                cout << "Error Message: Illegal/Unknown command." << endl;
                break;
        }
    }

}

int main(){
    int xpos = 0;
    int ypos = 0;
    int battery = 40;
    bool quit = false;

    string direction = "East";
    string input_mission = "dummyvalue";
    int goalx, goaly;
    int success_comms = 0;
    while (!quit){
        cout << "Input misson:" << endl;
        cin >> input_mission;
        if (input_mission == "quit"){
            quit = true;
        }
        else{
            cout << "Input goal x coordinate:";
            cin >> goalx;
            cout << "Input goal y coordinate:";
            cin >> goaly; 
            
            command(input_mission, xpos, ypos, direction, battery, success_comms);
            mission_complete_message(xpos, ypos, goalx, goaly, battery, success_comms, direction);
        }
    }
    return 0;
}