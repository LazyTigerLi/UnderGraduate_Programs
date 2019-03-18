`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:32:48 05/16/2018 
// Design Name: 
// Module Name:    MEM_WB 
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
module MEM_WB(
	input clk,
	input rst_n,
	input [31:0] rData_mem,
	input [31:0] ALU_result,
	input [4:0] wAddr_reg,
	
	input RegWrite,
	input MemtoReg,
	
	output reg [31:0] rData_mem_out,
	output reg [31:0] ALU_result_out,
	output reg [4:0] wAddr_reg_out,
	
	output reg RegWrite_out,
	output reg MemtoReg_out
    );
	 always @ (negedge clk or negedge rst_n)
	 begin
		if(~rst_n)
		begin
			rData_mem_out <= 32'h0;
			ALU_result_out <= 32'h0;
			wAddr_reg_out <= 5'h0;
		
			RegWrite_out <= 1'b0;
			MemtoReg_out <= 1'b0;
		end
		else
		begin
			rData_mem_out <= rData_mem;
			ALU_result_out <= ALU_result;
			wAddr_reg_out <= wAddr_reg;
			
			RegWrite_out <= RegWrite;
			MemtoReg_out <= MemtoReg;
		end
	 end
endmodule
