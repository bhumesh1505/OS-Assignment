/* Ryan Kadri, Austin Su, Nyambi Acho, Utkarsha Mavani*/
#include "mathfs.h"

char* opAdd(const char* input){

	char* result = (char*) malloc(64*sizeof(char));

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt = -1;
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==2){
		if(doubleOrInt == 1){
			sprintf(result,"%ld\n",inInts[0] + inInts[1]);
		}else{
			sprintf(result,"%f\n",inDoubles[0] + inDoubles[1]);
		}
	}else{
		sprintf(result, "Invalid Input\n");
	}
	return result;

}

char* opSub(const char* input){

	char* result = (char*) malloc(64*sizeof(char));

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt = -1;
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==2){
		if(doubleOrInt == 1){
			sprintf(result,"%ld\n",inInts[0] - inInts[1]);
		}else{
			sprintf(result,"%f\n",inDoubles[0] - inDoubles[1]);
		}
	}else{
		sprintf(result, "Invalid Input\n");
	}
	return result;

}

char* opMul(const char* input){

	char* result = (char*) malloc(64*sizeof(char));

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt = -1;
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==2){
		if(doubleOrInt == 1){
			sprintf(result,"%ld\n",inInts[0] * inInts[1]);
		}else{
			sprintf(result,"%f\n",inDoubles[0] * inDoubles[1]);
		}
	}else{
		sprintf(result, "Invalid Input\n");
	}
	return result;

}

char* opDiv(const char* input){

	char* result = (char*) malloc(64*sizeof(char));

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt = 1;
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==2){
		double res = inDoubles[0] / inDoubles[1];
		if(inDoubles[1] == 0.0){
			sprintf(result,"can not do divide by Zero\n");
		}else{
			sprintf(result,"%f\n",res);
		}
	}else{
		sprintf(result, "Invalid Input\n");
	}
	return result;

}

char* opExp(const char* input){

	char* result = (char*) malloc(64*sizeof(char));

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt = -1;
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==2){
		if(doubleOrInt == 1){
			if(inInts[0] == 0 && inInts[2] < 0){
				sprintf(result,"Divide by zero\n");
			}
			/* Uses the math.h pow() function to make num^decimal calculations easier*/
			sprintf(result,"%ld\n",(long) round(pow(inInts[0] , inInts[1])));
		}else{
			double res = pow(inDoubles[0] , inDoubles[1]);
			sprintf(result,"%f\n",res);
		}
	}else{
		sprintf(result, "Invalid Input\n");
	}
	return result;

}

char* opFactor(const char* input){

	long inInts[2];
	double inDoubles[2];
	int doubleOrInt=-1;
	
	/* Buffer for error messages*/
	char* result = (char*) malloc(32*sizeof(char));
	result[0] = '\0';
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==1){
		if(doubleOrInt == 0){
			sprintf(result,"Must factor integers\n");
		}else{
			/* We'll be creating a different buffer if input is good. So
			 * free the old one*/
			free(result);
			return factorThis(inInts[0]);
		}
	}else{
		sprintf(result,"Bad input");
	}
		
	return result;

}

char* factorThis(long input){
	
	char* buf = (char*) malloc(512*sizeof(char));
	buf[0] = '\0';
	
	if(input == 0 || input == 1){
		sprintf(buf,"%ld\n",input);
		return buf;
	}
	
	if(input < 0){
		input *= -1;
		strcat(buf,"-1\n");
	}
	
	int curFactor = 2;
	while(input > 1){
		while(input % curFactor == 0){
			input/=curFactor;
			sprintf(buf + strlen(buf),"%d\n", (int) curFactor);
		}
		++curFactor;
	}
	
	return buf;
}

int parseOperation(const char* input, long* inInts, double* inDoubles, int* doubleOrInt){
	
	int i,len;
	len = strlen(input);
	
	if(strchr(input, '.') == NULL && *doubleOrInt != 1){
		(*doubleOrInt) = 1;
		inDoubles = NULL;
		inInts[0] = 0; inInts[1] = 0;
	}else{
		(*doubleOrInt) = 0;
		inInts = NULL;
		inDoubles[0] = 0; inDoubles[1] = 0;
	}

	if(len == 0 || input[0]!='/'){
		return -1;
	}
	
	char* start = input + 1;
	char* end = strchr(start,'/');
	
	if(end != NULL){
		(*end) = '\0';
	}

	if(*doubleOrInt == 1){
		int firstNum = strtol(start,NULL,10);
		inInts[0] = firstNum;
	}else{
		double firstNum = strtod(start,NULL);
		inDoubles[0] = firstNum;
	}
	
	if(end == NULL){
		return 1;
	}

	start = end + 1;
	if(*start == '\0'){
		return -1;
	}
	
	if(*doubleOrInt == 1){
		int secondNum = strtol(start,NULL,10);
		inInts[1] = secondNum;
	}else{
		double secondNum = strtod(start,NULL);
		inDoubles[1] = secondNum;
	}
	
	return 2;
}

char* opFib(const char* input){
	
	long inInts[2];
	double inDoubles[2];
	int doubleOrInt=-1;
	char* result = (char*) malloc(32*sizeof(char));
	result[0] = '\0';
	
	if(parseOperation(input,inInts,inDoubles,&doubleOrInt)==1){
		if(doubleOrInt == 0){
			sprintf(result,"Only Integer Inputs Valid\n");
		}else{
			free(result);
			return fibThis(inInts[0]);
		}
	}else{
		sprintf(result,"Bad input");
	}
		
	return result;
}

char* fibThis(long input){
	char* buf = (char*) malloc(512*sizeof(char));
	buf[0] = '\0';
	
	if(input >= 1){
		sprintf(buf,"%ld\n",1L);
	}
	if(input >=2 ){
		sprintf(buf + strlen(buf),"%ld\n",1L);
	}
	
	long curFib, temp, cur = 1, prev = 1;
	
	for(curFib = 3; curFib <= input; ++curFib){
		temp = prev;
		prev = cur;
		cur = cur + temp;
		sprintf(buf + strlen(buf),"%ld\n",cur);
	}
	
	return buf;
}
