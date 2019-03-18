`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:33:01 05/16/2018 
// Design Name: 
// Module Name:    top 
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
module top(
	input clk,
	input rst_n,
	input display,
	input [7:0] addr_sel,
	
	output [31:0] program_counter,
	output [31:0] IR,
	output [31:0] IR2,
	
	
	/*output flush,
	output ne,
	output eq,
	output gez,
	output lez,
	output ltz,
	output carry,
	output [1:0] jump,*/

	output [3:0] sel_seg,
	output [7:0] out_data
    );
	 //wire [31:0] program_counter;
	 wire [4:0] wAddr_reg;
	 wire [31:0] wData_reg;
	 wire wEn_reg;
	 wire [31:0] rData1_reg;
	 wire [31:0] rData2_reg;
	
	 wire [31:0] ALU_result;
	
	 wire MemWrite;
	 wire RegWrite;
	 wire MemtoReg;
	
	
	 wire [31:0] wData_mem;
	 //wire [31:0] IR;
	 //wire [31:0] IR2;
	 
	 wire flush;
	 wire gtz;
	 wire ne;
	 wire eq;
	 wire gez;
	 wire lez;
	 wire ltz;
	 wire carry;
	 wire [1:0] jump;
	 
	 wire RegDst;
	 wire ALUSrc;
	 wire Branch_gtz;
	 wire Branch_ne;
	 wire Branch_eq;
	 wire Branch_gez;
	 wire Branch_lez;
	 wire Branch_ltz;
	 
	 wire [31:0] rData_mem;
	 wire [1:0] ALUSrcA_forward;
	 wire [1:0] ALUSrcB_forward;
	 
	 wire [31:0] new_pc;
	 wire [31:0] new_pc1;
	 wire [31:0] new_pc2;
	 wire [31:0] new_pc3;
	 wire [31:0] new_pc4;
	 wire [31:0] new_pc5;
	 //wire [31:0] IR2;
	 wire [31:0] IR3;
	 wire [31:0] rData1_reg_copy;
	 wire [31:0] rData2_reg_copy;
	 wire RegDst_copy;
	 wire ALUSrc_copy;
	 wire Branch_gtz_copy;
	 wire Branch_ne_copy;
	 wire Branch_eq_copy;
	 wire Branch_gez_copy;
	 wire Branch_lez_copy;
	 wire Branch_ltz_copy;
	 wire MemWrite_copy;
	 wire RegWrite_copy;
	 wire MemtoReg_copy;
	 
	 wire [31:0] ALUSrcA;
	 wire [31:0] ALUSrcB;
	 wire [4:0] wAddr_reg_copy;
	 wire [31:0] ALU_result_copy;
	 wire [31:0] rData2_reg_copy2;
	 wire [4:0] wAddr_reg_copy2;
	 wire gtz_copy;
	 wire ne_copy;
	 wire eq_copy;
	 wire gez_copy;
	 wire lez_copy;
	 wire ltz_copy;
	 wire Branch_gtz_copy2;
	 wire Branch_ne_copy2;
	 wire Branch_eq_copy2;
	 wire Branch_gez_copy2;
	 wire Branch_lez_copy2;
	 wire Branch_ltz_copy2;
	 wire MemWrite_copy2;
	 wire RegWrite_copy2;
	 wire MemtoReg_copy2;
	 
	 wire [31:0] rData_mem_copy;
	 wire [31:0] ALU_result_copy2;
	 wire MemtoReg_copy3;
	 
	 wire [1:0] rData2_reg_Src;
	 wire [31:0] rData2_reg_forward;
	 wire RegWriteSrc;
	 wire RegWrite_copy3;
	 
	 wire [4:0] Rt;
	 wire wData_mem_Src;
	 
	 wire PC_write;
	 wire IF_ID_write;
	 
	 wire RegWrite_hazard;
	 wire MemWrite_hazard;
	 
	 wire [31:0] real_pc;
	 wire [31:0] data_in_seg;
	
	 wire [7:0] addr;
	 wire [7:0] real_addr;
	 
	 
	 seg7_decoder seg(data_in_seg,clk,sel_seg,out_data);
	 
	 Display dis(clk,display,addr_sel,real_addr);
	 
	 mux3_1 mux10(new_pc,{{6{1'b0}},IR2[25:0]}<<2,rData1_reg,jump,real_pc);
	 PC pc(clk,rst_n,real_pc,PC_write,program_counter);
	 Instruction_rom instruction(clk,1'b0,program_counter[7:0] >> 2,32'h0,IR);
	 Add add1(program_counter,32'h4,new_pc1);
	 mux2_1 mux3(new_pc1,new_pc5,(Branch_gtz_copy2 & gtz_copy)||(Branch_ne_copy2 & ne_copy)||
					(Branch_eq_copy2 & eq_copy)||(Branch_gez_copy2 & gez_copy)
					||(Branch_lez_copy2 & lez_copy)||(Branch_ltz_copy2 & ltz_copy),new_pc);

	 IF_ID if_id(clk,new_pc1,IR,IF_ID_write,new_pc2,IR2);
	 REG_FILE regfile(clk,rst_n,IR2[25:21],IR2[20:16],wAddr_reg,wData_reg,wEn_reg,rData1_reg,rData2_reg);
	 control ctrl(IR2[31:26],IR2[5:0],rst_n,RegDst,ALUSrc,MemWrite,RegWrite,MemtoReg,Branch_gtz,Branch_ne,
						Branch_eq,Branch_gez,Branch_lez,Branch_ltz,jump);
	 Forwarding forward(wAddr_reg_copy2,wAddr_reg,IR3[25:21],IR3[20:16],RegWrite_copy,MemtoReg_copy,MemWrite_copy,
					Branch_gtz_copy,Branch_ne_copy,Branch_eq_copy,Branch_gez_copy,Branch_lez_copy,Branch_ltz_copy,
					RegWrite_copy2,MemtoReg_copy2,MemWrite_copy2,wEn_reg,MemtoReg_copy3,ALUSrc_copy,Rt,
					ALUSrcA_forward,ALUSrcB_forward,rData2_reg_Src,RegWriteSrc,wData_mem_Src);
	 
	 
	 Hazard_Detection hazard(RegWrite_copy & MemtoReg_copy,IR3[20:16],IR2[25:21],IR2[20:16],IR2[31:26],
									(Branch_eq_copy|Branch_gtz_copy|Branch_ne_copy|Branch_gez_copy|Branch_lez_copy|Branch_ltz_copy),
									(Branch_eq_copy2|Branch_gtz_copy2|Branch_ne_copy2|Branch_gez_copy2|Branch_lez_copy2|Branch_ltz_copy2),
									(Branch_gtz_copy2 & gtz_copy)||(Branch_ne_copy2 & ne_copy)||
									(Branch_eq_copy2 & eq_copy)||(Branch_gez_copy2 & gez_copy)
									||(Branch_lez_copy2 & lez_copy)||(Branch_ltz_copy2 & ltz_copy),
									PC_write,IF_ID_write,flush);
	 mux2_1_1bit mux8(MemWrite,32'b0,flush,MemWrite_hazard);
	 mux2_1_1bit mux9(RegWrite,32'b0,flush,RegWrite_hazard);
	 ID_EX id_ex(clk,rst_n,new_pc2,IR2,rData1_reg,rData2_reg,RegDst,ALUSrc,
						Branch_gtz,Branch_ne,Branch_eq,Branch_gez,Branch_lez,Branch_ltz,
						MemWrite_hazard,RegWrite_hazard,MemtoReg,
						new_pc3,IR3,rData1_reg_copy,rData2_reg_copy,RegDst_copy,ALUSrc_copy,
						Branch_gtz_copy,Branch_ne_copy,Branch_eq_copy,Branch_gez_copy,Branch_lez_copy,Branch_ltz_copy,
						MemWrite_copy,RegWrite_copy,MemtoReg_copy);
	 ALU alu(IR3[31:26],IR3[10:6],IR3[5:0],ALUSrcA,ALUSrcB,ALU_result,gtz,ne,eq,gez,lez,ltz,carry);			//ALUSrcA忘记改了，折腾了好久
	 mux3_1 mux0(rData1_reg_copy,wData_reg,ALU_result_copy,ALUSrcA_forward,ALUSrcA);
	 mux4_1 mux1(rData2_reg_copy,{{16{IR3[15]}},IR3[15:0]},ALU_result_copy,wData_reg,ALUSrcB_forward,ALUSrcB);
	 Add add2(new_pc3,{{16{IR3[15]}},IR3[15:0]}<<2,new_pc4);
	
	
	 mux3_1 mux5(rData2_reg_copy,wData_reg,ALU_result_copy,rData2_reg_Src,rData2_reg_forward);
	 EX_MEM ex_mem(clk,rst_n,new_pc4,ALU_result,rData2_reg_forward,wAddr_reg_copy,
				gtz,ne,eq,gez,lez,ltz,
				Branch_gtz_copy,Branch_ne_copy,Branch_eq_copy,Branch_gez_copy,Branch_lez_copy,Branch_ltz_copy,
				MemWrite_copy,RegWrite_copy,MemtoReg_copy,IR3[20:16],new_pc5,ALU_result_copy,rData2_reg_copy2,wAddr_reg_copy2,
				gtz_copy,ne_copy,eq_copy,gez_copy,lez_copy,ltz_copy,
				Branch_gtz_copy2,Branch_ne_copy2,Branch_eq_copy2,Branch_gez_copy2,Branch_lez_copy2,Branch_ltz_copy2,
				MemWrite_copy2,RegWrite_copy2,MemtoReg_copy2,Rt);
	 mux2_1_5bit mux2(IR3[20:16],IR3[15:11],RegDst_copy,wAddr_reg_copy);
	 
	 
	 mux2_1 mux7(rData2_reg_copy2,wData_reg,wData_mem_Src,wData_mem);
	 Data_ram data(clk,MemWrite_copy2,ALU_result_copy[7:0]>>2,wData_mem,rData_mem,
						clk,1'b0,real_addr,32'h0,data_in_seg);
	 
	 mux2_1_1bit mux6(RegWrite_copy2,1'b0,RegWriteSrc,RegWrite_copy3);
	 MEM_WB mem_wb(clk,rst_n,rData_mem,ALU_result_copy,wAddr_reg_copy2,RegWrite_copy3,MemtoReg_copy2,rData_mem_copy,
						ALU_result_copy2,wAddr_reg,wEn_reg,MemtoReg_copy3);
	 mux2_1 mux4(ALU_result_copy2,rData_mem_copy,MemtoReg_copy3,wData_reg);
	 
endmodule



//hahaha,流水线成功，成就感爆棚，yeyeye