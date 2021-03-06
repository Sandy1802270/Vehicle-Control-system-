#include <stdio.h>
#define ON 1
#define OFF 0
#define WITH_ENGINE_TEMP_CONTROLLER 1//initially defined with one

void main_menu(char *inputptr){
	do {//to ensure valid response from user
		printf("a. Turn on the vehicle engine\n");
		printf("b. Turn off the vehicle engine\n");
		printf("c. Quit the system\n\n");
		scanf(" %c",inputptr);
	}while ((*inputptr!='a')&&(*inputptr!='b')&&(*inputptr!='c'));
}

void main_menu_choice(int* quitflag,char * input, int*engine_state){
	if (*input=='c'){
		*quitflag=1;//set flag to end program in main
	}
	else if (*input=='b'){
		printf("are you sure? please type the state you want again\n");
		scanf(" %c",input);
		if (*input=='b'){
			*engine_state=OFF;
			printf("the vehicle engine is turned OFF\n\n");
			main_menu(input);//return to main menu
			main_menu_choice(quitflag,input, engine_state);
		}
		else{
			main_menu_choice(quitflag,input, engine_state);
		}
	}
	else{
		*engine_state=ON;
		printf("the vehicle engine is turned ON\n\n");
	}
}

void processing_and_display(int vehicle_speed,int engine_state,int*AC, float*room_temperature,int*engine_temp_controller,float*engine_temp  ){
	if (vehicle_speed==30){
		if (*AC==OFF)
			*AC=ON;
		*room_temperature=(*room_temperature)*1.25+1;
#if (WITH_ENGINE_TEMP_CONTROLLER==1)//skip this part of code if WITH_ENGINE_TEMP_CONTROLLER defined as 0
		if(*engine_temp_controller==OFF)
			*engine_temp_controller=ON;
		*engine_temp=(*engine_temp)*1.25+1;
#endif
	}
	if (engine_state==ON)
		printf("Engine is ON\n");
	else
		printf("Engine is OFF\n");
	if (*AC==ON)
		printf("AC is ON\n");
	else
		printf("AC is OFF\n");
	printf("Vehicle speed: %d Km/Hr\n",vehicle_speed);
	printf("Room temperature: %.3f C\n",*room_temperature);
#if (WITH_ENGINE_TEMP_CONTROLLER==1)//skip this part of code if WITH_ENGINE_TEMP_CONTROLLER defined as 0
	if (*engine_temp_controller==ON)
		printf("Engine temperature controller is ON\n");
	else
		printf("Engine temperature controller is OFF\n");
	printf("Engine temperature: %.3f C\n\n",*engine_temp);
#endif
}

void sensor_set_menu(){
	printf("Sensors set menu :\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature (Temperature Sensor)\n");
#if (WITH_ENGINE_TEMP_CONTROLLER==1)//skip this part of code if WITH_ENGINE_TEMP_CONTROLLER defined as 0
	printf("d. Set the engine temperature (Engine Temperature Sensor)\n\n");
#endif
}

int set_room_temperature(float *room_temp_ptr){
	printf("Enter the sensor read room temperature\n");
	scanf("%f",room_temp_ptr);
	if ((*room_temp_ptr<10)||(*room_temp_ptr>30)){
		*room_temp_ptr=20;
		return 1; //turn AC on
	}
	else{
		return 0;//turn AC off
	}
}

int set_vehicle_speed(){
	char traffic_light;
	printf("Enter the sensor read color\n");
	do {
		scanf("%c", &traffic_light);
	}while((traffic_light!='G')&&(traffic_light!='O')&&(traffic_light!='R'));
	switch(traffic_light){
	case 'G':return 100;
	break;
	case 'O': return 30;
	break;
	case 'R': return 0;
	break;
	}
	return 0;
}

int set_engine_temperature(float *engine_temp_ptr){
	printf("Enter the sensor read engine temperature\n");
	scanf("%f",engine_temp_ptr);
	if ((*engine_temp_ptr<100)||(*engine_temp_ptr>150)){
		*engine_temp_ptr=125;
		return 1;
	}
	else{
		return 0;
	}
}

int main(void){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0); // these two lines for printing message problem in eclipse
	int engine_state=OFF,vehicle_speed=35, AC=OFF,engine_temp_controller=OFF; //initialization of states
	float room_temperature=35, engine_temp=90; //initialization of temperature
	int quitflag=0;//flag is set if the user choose quit to return 0 in main and end program
	char input, sensor_menu_input;
	main_menu(&input);
	main_menu_choice(&quitflag, &input, &engine_state);
	if (quitflag==1)
		return 0;//end program
	while (engine_state==ON){
		sensor_set_menu();
		do {//take inputs as long as the user doesn't enter valid input
			scanf(" %c",&sensor_menu_input);
		}while((sensor_menu_input!='a')&&(sensor_menu_input!='b')&&(sensor_menu_input!='c')&&(sensor_menu_input!='d'));
		switch (sensor_menu_input){
		case 'a':
			engine_state=OFF;
			printf("the vehicle engine is turned OFF\n\n");
			main_menu(&input);//return to main menu
			main_menu_choice(&quitflag, &input, &engine_state);
			if (quitflag==1)
				return 0;
			break;
		case 'b':
			vehicle_speed=set_vehicle_speed();
			processing_and_display(vehicle_speed,engine_state,&AC, &room_temperature,&engine_temp_controller,&engine_temp  );
			break;
		case 'c':
			AC=set_room_temperature(&room_temperature);
			processing_and_display(vehicle_speed,engine_state,&AC, &room_temperature,&engine_temp_controller,&engine_temp  );
			break;
		case 'd':
#if (WITH_ENGINE_TEMP_CONTROLLER==1)//skip this part of code if WITH_ENGINE_TEMP_CONTROLLER defined as 0
			engine_temp_controller=set_engine_temperature(&engine_temp);
			processing_and_display(vehicle_speed,engine_state,&AC, &room_temperature,&engine_temp_controller,&engine_temp  );
#endif
			break;
		}
	}

}


