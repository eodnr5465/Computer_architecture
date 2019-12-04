#include <iostream>
#include <fstream>
#include <cstdlib>

#include "CCode.h"
#include "CDecode.h"
#include "CExecute.h"
#include "CRegister.h"
#include "CMemory.h"

using namespace std;

int main( int argc, char* argv[]){
	if(argc != 3){

		cout << "tpu <input file> <line>\n";
		return -1;
		}
	CFlash1KWord                code_memory(argv[1], atoi(argv[2]));

   
	for(int i=0; i<atoi(argv[2]); i++){
		cout << code_memory.code_at(i) << endl;
		}

    CT1DecodeDirectFetch         decode(code_memory);
    C16RegisterFile              regs;
    CSRAM_256W                   mems;
    CT1ExecuteTinyUnit           execute(decode, regs, mems);
    

	int size = atoi(argv[2]);
	int total_clock = 0;
  //  for(int i = 0; i< atoi(argv[2]); i++){
  	while(regs.get_PC() < size){

        decode.do_fetch_from(regs.get_PC());
        decode.do_decode();
        decode.show_instruction();

        total_clock += execute.do_execute();


    }

    cout << "After executing instruction ... & regsterfile contants" << endl;
    cout << "Total Clocks : " << total_clock << endl;
    regs.show_regs();

    mems.show_mems(1, 25);
    mems.show_mems(26,50);
    mems.show_mems(51,75);
	
	}
