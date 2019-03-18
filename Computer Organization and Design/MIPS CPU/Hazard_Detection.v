`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:59:53 05/29/2018 
// Design Name: 
// Module Name:    Hazard_Detection 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Hazard_Detection(
	input ID_EX_MemRead,
	input [4:0] ID_EX_Rt,
	input [4:0] IF_ID_Rs,
	input [4:0] IF_ID_Rt,
	input [5:0] IF_ID_Opcode,
	input ID_EX_Branch,
	input EX_MEM_Branch,
	input brk,
	output reg PC_write,
	output reg IF_ID_write,
	output reg flush
    );
	 parameter bgtz = 6'b000111;
	 parameter beq = 6'h4;
	 parameter bgez = 6'h1;
	 parameter blez = 6'h6;
	 parameter bltz = 6'h1;
	 parameter bne = 6'h5;
	 always @ (*)
	 begin 	
			if((ID_EX_MemRead == 1'b1 && (ID_EX_Rt == IF_ID_Rs || ID_EX_Rt == IF_ID_Rt)) || 
				(ID_EX_Branch == 1'b1 || IF_ID_Opcode == bgtz || IF_ID_Opcode == bne || 
				IF_ID_Opcode == beq || IF_ID_Opcode == bgez || IF_ID_Opcode == blez || IF_ID_Opcode == bltz))
				IF_ID_write = 1'b0;
			else IF_ID_write = 1'b1;
			
			if(EX_MEM_Branch == 1'b1 && brk == 1'b0)IF_ID_write = 1'b1;
			if(EX_MEM_Branch == 1'b1 && brk == 1'b1)IF_ID_write = 1'b0;
			
			if((ID_EX_MemRead == 1'b1 && (ID_EX_Rt == IF_ID_Rs || ID_EX_Rt == IF_ID_Rt))
				|| (ID_EX_Branch == 1'b1 || IF_ID_Opcode == bgtz || IF_ID_Opcode == bne || 
				IF_ID_Opcode == beq || IF_ID_Opcode == bgez || IF_ID_Opcode == blez || IF_ID_Opcode == bltz))
				PC_write = 1'b0;
			else PC_write = 1'b1;
			
			if((ID_EX_MemRead == 1'b1 && (ID_EX_Rt == IF_ID_Rs || ID_EX_Rt == IF_ID_Rt)) ||
				(ID_EX_Branch == 1'b1 || EX_MEM_Branch == 1'b1))
				flush = 1'b1;
			else flush = 1'b0;		
	 end
endmodule
