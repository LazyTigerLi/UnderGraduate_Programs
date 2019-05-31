`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB（Embeded System Lab�?
// Engineer: Haojun Xia
// Create Date: 2019/03/14 11:21:33
// Design Name: RISCV-Pipline CPU
// Module Name: NPC_Generator
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Choose Next PC value
//////////////////////////////////////////////////////////////////////////////////
module NPC_Generator(
    input wire [31:0] PCF,JalrTarget, BranchTarget, JalTarget,
    input wire BranchE,JalD,JalrE,
    input wire foundF,
    input wire [31:0] npc_predicted,
    
    input wire [31:0] PCE,
    input wire mispredicted,
    input wire foundE,
    output reg [31:0] PC_In
    );
    always @(*)
    begin
        if(foundE)              //BTB����
        begin
            if(mispredicted & BranchE)      //Ԥ����󣬲���ȷʵҪ��ת����Ԥ�����ת��ַ����
                PC_In <= BranchTarget;
            else if(mispredicted & ~BranchE)    //����ת����Ԥ����ת
                PC_In <= PCE + 4;
            else if(foundF)                 //Ԥ����ȷ
                 PC_In <= npc_predicted;
            else
                 PC_In <= PCF+4;
        end else begin
            if(BranchE)        //BTBû������
                PC_In <= BranchTarget;
            else if(JalrE)
                PC_In <= JalrTarget;
            else if(JalD)
                PC_In <= JalTarget;
            else if(foundF)
                PC_In <= npc_predicted;
            else
                PC_In <= PCF+4;
        end
    end
endmodule
