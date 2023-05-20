// AstroLibExamples.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include "AstroLibExamples.h"
#include "ExampleOrbElemConv.h"
#include "ExampleMath.h"
#include "ExampleOrbProp.h"
#include "ExampleAttitude.h"
#include "ExampleODEResolve.h"



void main()
{
	printf("*****************************************************************\n");
	printf("* AstroLib Example\n");
	printf("*****************************************************************\n\n");

	int sel;

	while (true)
	{
		printf("\n*****************************************************************\n"); 
		printf("Please select example program to run...\n"); 
		printf("*****************************************************************\n"); 
		printf("[1]. Orbit Element Conversion\n");
		printf("[2]. Math\n");
		printf("[3]. Orbit Propagation\n");
		printf("[4]. Attitude\n");
		printf("[5]. Ordinary Differential Equation\n");
		printf("[6]. Exit\n\n");
		printf("Your select is?\n>> "); 

		scanf("%d", &sel);
		switch(sel)
		{
		case 1:
			ExampleOrbElemConv();
			break;
		case 2:
			ExampleMath();
			break;
		case 3:
			ExampleOrbProp();
			break;
		case 4:
			ExampleAttitude();
			break;
		case 5:
			ExampleODEResolve();
			break;
		default:
			cout<<"\nError select!\n";
		}
	}
	
}


