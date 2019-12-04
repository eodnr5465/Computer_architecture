#include "CExecute.h"

int clks[10] = {8, 8, 12, 6, 4, 4, 12, 30, 2, 12};
int CT1ExecuteTinyUnit :: do_execute(){


    //MOV3 R1, #2
    if(m_decode_unit.get_opcode() == MOV3){
        unsigned int reg_index =    m_decode_unit.get_op1();
                 int      data =    m_decode_unit.get_op2();

        //write into register
        m_regs.write_on_reg(reg_index, data);
        
		m_regs.set_PC(m_regs.get_PC()+1);
        return clks[MOV3];

    }

    // ADD R0, R1 --> R0 = R0 + R1
    else if(m_decode_unit.get_opcode() == ADD){
        unsigned int reg_n = m_decode_unit.get_op1();
        unsigned int reg_m = (m_decode_unit.get_op2()>>4) & 0xf;
       
        //read form register
        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);
    
        // add operate
        Rn = Rn + Rm;

        //write into register
        m_regs.write_on_reg(reg_n, Rn);
 
		m_regs.set_PC(m_regs.get_PC()+1);
        return clks[ADD];

    }
        
    
    // SUB R0, R1 --> R0 = R0 - R1
    else if(m_decode_unit.get_opcode() == SUB){
        unsigned int reg_n = m_decode_unit.get_op1();
        unsigned int reg_m = (m_decode_unit.get_op2()>>4) & 0xf;

        //read from register for operating
        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);

        // sub operate
        Rn = Rn - Rm;

		//write into register
		m_regs.write_on_reg(reg_n, Rn);

    	m_regs.set_PC(m_regs.get_PC()+1);
        
        // after operating, write into register 
        m_regs.write_on_reg(reg_n, Rn);
		
		return clks[SUB];
    }
	
    // MOV0 R1, [3] : R1 <- M[3]  
    else if(m_decode_unit.get_opcode() == MOV0) {
        unsigned int reg_n      = m_decode_unit.get_op1();
        unsigned int mem_addr   = m_decode_unit.get_op2() & 0xff;

        // read memory data from CSRAM_256W
        int memory_data = m_mems.read_from_memory(mem_addr);
        
        // write on register from memory
        m_regs.write_on_reg(reg_n, memory_data);

    	m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MOV0];

    }
	
	//  MOV2 R1, R2 : R1 -> M[3] 

   else if(m_decode_unit.get_opcode() == MOV2) {
        unsigned int reg_n   = m_decode_unit.get_op1();
        unsigned int reg_m   = m_decode_unit.get_op2() & 0x0f;

        // read register data 
        int reg_m_data = m_regs.read_from_reg(reg_m);
        int reg_n_data = m_regs.read_from_reg(reg_n);

        //write n_data = address & address data = m_data
        m_mems.write_on_memory(reg_n_data, reg_m_data);

		m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MOV2];

    }
	// MOV4 Rn, Rm : Rm_data copy to Rn

   else if(m_decode_unit.get_opcode() == MOV4) {
        unsigned int reg_n   = m_decode_unit.get_op1();
        unsigned int reg_m   = (m_decode_unit.get_op2()>>4) & 0x0f;

        // read register data 
        int reg_m_data = m_regs.read_from_reg(reg_m);

        //copt to Rn

        m_regs.write_on_reg(reg_n , reg_m_data);

		m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MOV4];

    }

    
    // MOV1 [3],R1  : R1 -> M[3]
    else if(m_decode_unit.get_opcode() == MOV1) {
        unsigned int reg_n      = m_decode_unit.get_op1();
        unsigned int mem_addr   = m_decode_unit.get_op2() & 0xff;

        //read nth register data for save to flash memory
        int reg_data = m_regs.read_from_reg(reg_n);

        //after read write on special addr which in CSRAM_256W from register
        m_mems.write_on_memory(mem_addr , reg_data);
	   
	    m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MOV1];
    }

    
    // MUL R0, R1 --> R0 = R0 * R1
    else if(m_decode_unit.get_opcode() == MUL){
        unsigned int reg_n = m_decode_unit.get_op1();
        unsigned int reg_m = (m_decode_unit.get_op2()>>4) & 0xf;
        
        //read form register
        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);
    
        //multiple operate
        Rn = Rn * Rm;

        //write into register
        m_regs.write_on_reg(reg_n, Rn);

		m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MUL];

    }

	 // ex. JZ R0, #3 (R0 == 0 , then PC+1+3)
   	 else if(m_decode_unit.get_opcode() == JZ){
      	unsigned int reg_n 	= m_decode_unit.get_op1();
        		 int offset = m_decode_unit.get_op2();
        
        //read form register
        int Rn = m_regs.read_from_reg(reg_n);
        m_regs.set_PC(m_regs.get_PC()+1);
    	
		if(Rn == 0 ) {
			int pc = m_regs.get_PC();
			m_regs.set_PC(pc + offset);
			}
			
      
        return clks[JZ];

    }


     // MOV5 R1, R2 : R1 <- M[R2]  
    else if(m_decode_unit.get_opcode() == MOV5) {
        unsigned int reg_n      = m_decode_unit.get_op1();
        unsigned int reg_m      = m_decode_unit.get_op2() & 0xf;

        // read register data from register file
        int reg_data = m_regs.read_from_reg(reg_m);

        // read memory data from CSRAM_256W
        int memory_data = m_mems.read_from_memory(reg_data);
        
        // write on register from memory
        m_regs.write_on_reg(reg_n, memory_data);

    	m_regs.set_PC(m_regs.get_PC()+1);

        return clks[MOV5];

    }    
    else{ 
        cout << "Not executable instruction, not yet implemented sorry.."<<endl;
        return false;
    }
    
}
