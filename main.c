//import libraries from the standard C library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//create constants that will be used in file management
#define LSIZ 128 
#define RSIZ 10 

//declare global variables to store final grade for each student
//that are reset when moved to next student
int creditHours = 0;
double totalCredits = 0;

//function declarations
void readStudent();
void readRegistration(char id[]);
void outputStudentFile(char id[], char nm[], char srnm[], char department[]);
void grading(char id[], int grade,int credits);
char * courseTitle(char title[]);
void registeredCourses(char id[],char year[]);
void getCriteria(char id[],char coursecode[],char value1[], char value2[], char value3[]);
void getPerformance(char id[],char coursecode[]);
char * parseYear(char year[]);
void getTotal(char id[]);

//main function
void main()
{
//call the readStudent function
readStudent();
printf("\n%s\n\n","Transcipts have been generated successfully!");

}

//readstudent function
// This function reads the student list and calls the outputStudent function
void readStudent(){
    char line[RSIZ][LSIZ];
    int i = 0;
    FILE *studfile = NULL;
    char studfilename[20]="./students.txt";
    studfile = fopen(studfilename, "r");
    while(fgets(line[i], LSIZ, studfile)) 
	{   
        char *ch;
        char *array[4];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL," ");
        }
        outputStudentFile(array[0],array[1],array[2],array[3]);
        creditHours = 0;
        totalCredits = 0;
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
}

//outputStudentFile function used to output to a file for each student
void outputStudentFile(char id[], char nm[], char srnm[], char department[]){

    char fid[10];
    memcpy(fid,id,10);
    FILE *indv;
    indv = fopen(strcat(fid,".txt"), "w");
    fprintf(indv,"--------------------------------------------------------------------------------\n\n");
    fprintf(indv,"Name: %s\n", nm);
    fprintf(indv,"Surname: %s\n", srnm);
    fprintf(indv,"Student Number: %s\n",id);
    fprintf(indv,"Department: %s\n", department);
    fprintf(indv,"--------------------------------------------------------------------------------\n");
    fclose(indv);
    readRegistration(id);  
}
//readRegistration reads the registered courses
void readRegistration(char id[]){
    char line[RSIZ][LSIZ];
	char fname[20]="./registrations.txt";
    FILE *fptr = NULL; 
    int i = 0;
    fptr = fopen(fname, "r");
    char year[10];
    while(fgets(line[i], LSIZ, fptr)) 
	{   
        char *ch;
        char *array[4];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL," ");
        }
        if(strcmp(year,array[1])!=0){
            memcpy(year,array[1],10);
            char fid[10];
            memcpy(fid,id,10);
            FILE *indv;
            indv = fopen(strcat(fid,".txt"), "a");
            fputs("\nAcademic Year: ",indv);
            fputs(parseYear(year),indv);
            char *sem;
            if(strcmp(array[2],"1")==0){
                sem="Fall";
            }
            else{
                sem ="Spring";
            }
            fputs("\tSemester: ",indv);
            fputs(sem,indv);
            fputs("\n\n--------------------------------------------------------------------------------\n",indv);
            fputs("\nCourse Code\t Course title\t\t\t Credit\t Grade\t Credit earned\n",indv);
            //printf("%s\n",id);
            fclose(indv);
            registeredCourses(id, year);
            getTotal(id);
        }
        else{
            //go to next year
        }
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    
}
//regosteredCourses function gets the registered courses by the 
//student and outputs the student file as well as call
//getPerformance function
void registeredCourses(char id[], char year[]){
    char line[RSIZ][LSIZ];
	char fname[20]="./registrations.txt";
    FILE *fptr = NULL; 
    int i = 0;
    fptr = fopen(fname, "r");
    while(fgets(line[i], LSIZ, fptr)) 
	{   
        char *ch;
        char *array[4];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL," ");
        }
        if(strcmp(array[0],id)==0 && strcmp(array[1],year)==0){
            int length;
            length = strlen(array[3]);
            array[3][length - 1] = '\0';
            char fid[10];
            memcpy(fid,id,10);
            FILE *indv2;
            indv2 = fopen(strcat(fid,".txt"), "a");
            fputs(array[3],indv2);
            fputs("\t\t",indv2);
            fputs(courseTitle(array[3]),indv2);
            fclose(indv2);
            getPerformance(id,array[3]);
        }
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
}

//courseTitle function gets the title of the course
//from the courses.txt file and returns the full course Title
char * courseTitle(char title[]){
    char line[RSIZ][LSIZ];
	char fname[20]="./courses.txt";
    FILE *fptr = NULL; 
    int i = 0;
    fptr = fopen(fname, "r");
    while(fgets(line[i], LSIZ, fptr)) 
	{   
        char *ch;
        char *array[4];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL,",");
        }
        if(strcmp(array[0],title)==0){
            char * string;
            string = malloc(sizeof(char)*100);
            strcat(string, array[1]);
            return string;
        }
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
}

//getPeformance function gets the perfornances
//from performances.txt and calls getCriteria to 
//get criteria form criteria.txt
void getPerformance(char id[],char coursecode[]){
    char line[RSIZ][LSIZ];
	char fname[20]="./performances.txt";
    FILE *fptr = NULL; 
    int i = 0;
    fptr = fopen(fname, "r");
    while(fgets(line[i], LSIZ, fptr)) 
	{   
        char *ch;
        char *array[10];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL," ");
        }
        if(strcmp(array[0],id)==0 && strcmp(array[1],coursecode)==0){
            getCriteria(id,coursecode,array[2],array[3],array[4]);
        }
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
}

//getCriteria function gets the criteria from file and outputs the criteria
//to the student transcript file
void getCriteria(char id[], char coursecode[],char value1[], char value2[], char value3[]){
    char line[RSIZ][LSIZ];
	char fname[20]="./criteria.txt";
    FILE *fptr = NULL; 
    int i = 0;
    fptr = fopen(fname, "r");
    while(fgets(line[i], LSIZ, fptr)) 
	{   
        char *ch;
        char *array[10];
        int i =0;
        ch =strtok(line[i]," ");
        while(ch!=NULL){
            array[i++] = ch;
            ch = strtok(NULL," ");
        }
        if(strcmp(array[0],coursecode)==0){
            char fid[10];
            memcpy(fid,id,10);
            FILE *indv;
            indv = fopen(strcat(fid,".txt"), "a");
            fputs("\t\t\t\t\t\t\t\t\t\t",indv);
            fputs(array[1],indv);
            fputs("\t\t",indv);
            fclose(indv);
            creditHours = creditHours+atoi(array[1]);
            int total = (atoi(value1)*atoi(array[2])/100)+(atoi(value2)*atoi(array[3])/100)+(atoi(value3)*atoi(array[4])/100);
            grading(id, total,atoi(array[1]));
        }
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
}

//grading function that translates marks to grades in
//form of letter and numeric grades
void grading(char id[], int grade,int credits){
    float numeric_grade;
    char *letter_grade;
    char fid[10];
    memcpy(fid,id,10);
    FILE *indv;
    indv = fopen(strcat(fid,".txt"), "a");
    if(grade>=90){
        numeric_grade = 4.0;
        letter_grade ="A";
    }
    else if(grade>=85){
        numeric_grade = 3.7;
        letter_grade ="A-";
    }
    else if(grade>=80){
        numeric_grade = 3.3;
        letter_grade ="B+";
    }
    else if(grade>=75){
        numeric_grade = 3.0;
        letter_grade ="B";
    }
    else if(grade>=70){
        numeric_grade = 2.7;
        letter_grade ="B-";
    }
    else if(grade>=65){
        numeric_grade = 2.3;
        letter_grade ="C+";
    }
    else if(grade>=60){
        numeric_grade = 2.0;
        letter_grade ="C";
    }
    else if(grade>=55){
        numeric_grade = 1.7;
        letter_grade ="C-";
    }
    else if(grade>=50){
        numeric_grade = 1.3;
        letter_grade ="D+";
    }
    else if(grade>=45){
        numeric_grade = 1.0;
        letter_grade ="D";
    }
    else if(grade>=40){
        numeric_grade = 0.7;
        letter_grade ="D-";
    }
    else{
        numeric_grade = 0.0;
        letter_grade ="E";
    }
    numeric_grade = numeric_grade*credits;
    totalCredits = totalCredits+numeric_grade;
    
    fputs(letter_grade,indv);
    fputs("\t\t",indv);
    char numgrade[50];
    sprintf(numgrade,"%.2f",numeric_grade);
    fputs(numgrade,indv);
    fputs("\n",indv);
    fclose(indv);
}

//ParseYear function is used to convert year to
//appropriate format
char * parseYear(char year[]){
    char a[15];
    char c[15];
    char b[2];
    memcpy(a,year,15);
    memcpy(c,a,15);
    memcpy( b, &a[4], 4);
    b[4] = '\0';
    c[4]='-';
    c[5]=0;
    strcat(c,b); 
    char * string;
    string = malloc(sizeof(char)*100);
    strcat(string, c);
    return string;
}

//getTotal function outputs the total and places it in a file
//together with the total credits and credit hours
void getTotal(char id[]){
    char fid[10];
    memcpy(fid,id,10);
    FILE *indv3;
    indv3 = fopen(strcat(fid,".txt"), "a");
    fputs("\n--------------------------------------------------------------------------------\n",indv3);
    char tc[10];
    char tch[10];
    char gpp[10];
    sprintf(tc,"%.1f",totalCredits);
    sprintf(tch,"%d",creditHours);
    double gpa;
    if(creditHours==0){
        gpa = 0.00;
    }
    else{
        gpa = totalCredits/creditHours;
    }
    sprintf(gpp,"%.2f",gpa);
    fputs("Total credit earned: ",indv3);
    fputs(tc,indv3);
    fputs("\n",indv3);
    fputs("Total credit hour: ",indv3);
    fputs(tch,indv3);
    fputs("\n",indv3);
    fputs("GPA:",indv3);
    fputs(gpp,indv3);
    fputs("\n",indv3);
    fputs("-------------------------------------------------------------------------------\n",indv3);
    fclose(indv3);
}