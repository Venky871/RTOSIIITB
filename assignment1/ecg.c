
#include<stdio.h>
#include<stdlib.h>
static  int  inp_n,inp_n1,inp_n2;     //input values of signal  inp_n2=inp[n-2];
static  int  out_n,out_n1,out_n2;     //filtered values of signal out_n1=out[n-1];
static int heart_beat;                //heart beat rate 
static  int  data,i;                   //temporary variables
static int temp_beat;                 //
static int temp_data;                   // 
static int temp_predata1;             //
static int temp_predata2;             //temporary variables
#define a   0.016                    // constants for filter
#define b   0.032                    // 
#define c   0.016                    //
#define d   1.63                     // constants for filter
#define e   0.691                  
int main()
{
	FILE *file_in,*file_out ;            // file pointers    
	file_in=fopen("sample.txt","r");     // open input file in read mode  
	file_out=fopen("output1.txt","w+");   // open output file in read and write mode
	for(i=0;i<5000;i++)                  // loop for reading values from input file 
	{                                     
		fscanf(file_in,"%d",&data);          // reding value from input file
		//printf("%d\n",data);
		inp_n2=inp_n1;                                                                     // algorithm for filter 
		inp_n1=inp_n;                                                                      //sampling frequency 112.5hz
		inp_n=data;                                                                        //cut off frequency 20hz
		out_n2=out_n1;                                                                     //second order butterworth filter
		out_n1=out_n;                                                                      //
		out_n=((a*inp_n)+(b*inp_n1)+(c*inp_n2)+(d*out_n1)-(e*out_n2));                                 // filter eqation
		fprintf(file_out,"%d\n",out_n);                                                    // printing filtered output in output file 
	}                                       
                                 
	fseek(file_out,800,SEEK_SET);                            // reading output file from 900 location to avoid intial disturbances 
	for(i=0;i<=3475;i++)                                                   // loop for reading data from output file
	{
		fscanf(file_out,"%d",&temp_data);                                     // reading output from file 
		if(temp_data>535)                                                     // algorithm for heart beat measurement
		{                                                                     //
			if((temp_data<temp_predata1)&(temp_predata1>=temp_predata2))  //taking 30 sec measurement and averaging it for 60Sec 
				temp_beat++;                                          //   
		}                                                                     //
		temp_predata2=temp_predata1;                                          //
		temp_predata1=temp_data;                                              //
		//printf("%d\n",temp_data);                                           //
	}                                                                     // 
	heart_beat=2*temp_beat;
	printf("%d\n",heart_beat);                                            // averaging for 60sec
	fclose(file_in);
        fclose(file_out);
                                                      // closing files 	     
FILE *pipe = popen("gnuplot -persist","w");
    fprintf(pipe, "set xrange [200.000:1000.000]\n");    // ploting in folder 
    fprintf(pipe, "set yrange [400.000:600.000]\n");     //
    fprintf(pipe, "set terminal png\n");                //   
    fprintf(pipe, "set output 'graph2.png'\n");         // name graph.png
   fprintf(pipe," plot 'output1.txt' with l  " );       // take from output
  fclose(pipe);

}
