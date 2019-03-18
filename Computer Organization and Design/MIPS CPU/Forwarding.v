`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:56:41 05/21/2018 
// Design Name: 
// Module Name:    Forwarding 
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

module Forwarding(
	input [4:0] EX_MEM_Rd,
	input [4:0] MEM_WB_Rd,
	input [4:0] ID_EX_Rs,
	input [4:0] ID_EX_Rt,
	
	input ID_EX_RegWrite,
	input ID_EX_MemtoReg,
	input ID_EX_MemWrite,
	input ID_EX_Branch_gtz,
	input ID_EX_Branch_ne,
	input ID_EX_Branch_eq,
	input ID_EX_Branch_gez,
	input ID_EX_Branch_lez,
	input ID_EX_Branch_ltz,
	
	input EX_MEM_RegWrite,
	input EX_MEM_MemtoReg,
	input EX_MEM_MemWrite,
	
	input MEM_WB_RegWrite,
	input MEM_WB_MemtoReg,
	
	input ID_EX_ALUSrcB,
	input [4:0] EX_MEM_Rt,
	
	output reg [1:0] ALUSrcA,
	output reg [1:0] ALUSrcB,
	output reg [1:0] rData2_reg_Src,
	output reg RegWriteSrc,
	output reg wData_mem_Src
    );
	 
	 always @ (*)
	 begin
			ALUSrcA = 2'b00;
			ALUSrcB = {1'b0,ID_EX_ALUSrcB};
			rData2_reg_Src = 2'b00;
			RegWriteSrc = 1'b0;
			wData_mem_Src = 1'b0;
			
			if(MEM_WB_Rd == ID_EX_Rs)
			begin
				if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b1
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b0)
					ALUSrcA = 2'b01;			//LW+R
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b0
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b1)
					ALUSrcA = 2'b01;			//R+LW
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b1
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b1)
					ALUSrcA = 2'b01;			//LW+LW
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b0 && ID_EX_MemWrite == 1'b1)
					ALUSrcA = 2'b01;			//R+SW
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b0		//R+?+R
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b0)
					ALUSrcA = 2'b01;
				else ALUSrcA = 2'b00;
			end
			
			if(MEM_WB_Rd == ID_EX_Rt)
			begin
				if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b1
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b0)
					begin
						ALUSrcB = 2'b11;			//LW+R		//һ��ʼд��ALUSrcA = 2'b11,�����˰���bug
						rData2_reg_Src = 2'b00;
					end
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b1
					&&  ID_EX_MemWrite == 1'b1)
					begin
						rData2_reg_Src = 2'b01;					//LW+?+SW ALUSrcA = 2'b0;			
						ALUSrcB = {1'b0,ID_EX_ALUSrcB};
					end
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b0
					&& ID_EX_MemWrite == 1'b1)
					begin
						rData2_reg_Src = 2'b01;
						ALUSrcB = {1'b0,ID_EX_ALUSrcB};
					end												//R+?+SW
				else if(MEM_WB_RegWrite == 1'b1 && MEM_WB_MemtoReg == 1'b0
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b1)
					begin
						rData2_reg_Src = 2'b00;
						ALUSrcB = 2'b11;							//R+?+R
					end
				else
					begin
						rData2_reg_Src = 2'b00;
						ALUSrcB = {1'b0,ID_EX_ALUSrcB};			//ԭ��д����ALUSrcB = 2'b00
					end
			end
			
			if(EX_MEM_Rd == ID_EX_Rs)
			begin
				if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b0)
					ALUSrcA = 2'b10;					//R+R
				else if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0 && ID_EX_MemWrite == 1'b1)
					ALUSrcA = 2'b10;					//R+SW
				else if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0 && 
					ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b1)
					begin
						ALUSrcA = 2'b10;					//R+LW //����LW��д�ؼĴ�����R��д�ؼĴ��������⣬���LW�������Ĵ�����ͬ�Ļ�
						if(ID_EX_Rs == ID_EX_Rt)RegWriteSrc = 1'b1;
						else RegWriteSrc = 1'b0;
					end
				else if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0 && 
						(ID_EX_Branch_eq | ID_EX_Branch_gez | ID_EX_Branch_gtz | ID_EX_Branch_lez
						| ID_EX_Branch_ltz | ID_EX_Branch_ne) == 1'b1)
					ALUSrcA = 2'b10;					//R+beq
				else ALUSrcA = 2'b00;
			end
			
			if(EX_MEM_Rd == ID_EX_Rt)
			begin
				if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0
					&& ID_EX_RegWrite == 1'b1 && ID_EX_MemtoReg == 1'b0)
				begin
					ALUSrcB = 2'b10;				//R+R
					rData2_reg_Src = 2'b00;
				end
				else if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0 && 
						((ID_EX_Branch_eq | ID_EX_Branch_ne) == 1'b1))	
				begin
					ALUSrcB = 2'b10;				//R+beq
					rData2_reg_Src = 2'b00;
				end
				else if(EX_MEM_RegWrite == 1'b1 && EX_MEM_MemtoReg == 1'b0 && ID_EX_MemWrite == 1'b1)
				begin
					rData2_reg_Src = 2'b10;
					ALUSrcB = {1'b0,ID_EX_ALUSrcB};
				end
				else 
				begin	
					ALUSrcB = {1'b0,ID_EX_ALUSrcB};
					rData2_reg_Src = 2'b00;
				end
			end
			if(EX_MEM_Rt == MEM_WB_Rd)
			begin
				if(MEM_WB_RegWrite == 1'b1 && EX_MEM_MemWrite == 1'b1)
					wData_mem_Src = 1'b1;
				else wData_mem_Src = 1'b0;
			end
			
								//�����ˣ���һ��addָ������R+LW,RegWrite���ó�1����lwָ����Ȼ��1.�����������һ�����ϣ�shit!!!
	 end
endmodule
