`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB锛圗mbeded System Lab锛?
// Engineer: Haojun Xia
// Create Date: 2019/03/14 11:21:33
// Design Name: RISCV-Pipline CPU
// Module Name: NPC_Generator
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Choose Next PC value
//////////////////////////////////////////////////////////////////////////////////
`define BHT

module NPC_Generator(
    input wire [31:0] PCF,JalrTarget, BranchTarget, JalTarget,
    input wire BranchE,JalD,JalrE,
    input wire foundF,
    input wire [31:0] npc_predicted,
    
    input wire [31:0] PCE,
    input wire mispredicted,
    input wire foundE,
    input wire takenF,
    output reg [31:0] PC_In
    );
    always @(*)
    begin
        //先处理正在EX段执行的分支指令是否预测正确
        if(foundE)              //BTB命中
        begin
            if(mispredicted & BranchE)      //预测错误，并且确实要跳转，即预测的跳转地址错误
                PC_In <= BranchTarget;
            else if(mispredicted & ~BranchE)    //不跳转，但预测跳转
                PC_In <= PCE + 4;
            `ifndef BHT
            else if(foundF)
                PC_In <= npc_predicted;
            `else
            else if(foundF & takenF)
                PC_In <= npc_predicted;
            `endif
            else
                PC_In <= PCF+4;
        end else begin
            if(BranchE)        //BTB没有命中
                PC_In <= BranchTarget;
            else if(JalrE)
                PC_In <= JalrTarget;
            else if(JalD)
                PC_In <= JalTarget;
            `ifndef BHT
            else if(foundF)
                PC_In <= npc_predicted;
            `else
            else if(foundF & takenF)
                PC_In <= npc_predicted;
            `endif
            else
                PC_In <= PCF+4;
        end
    end
endmodule
