`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB (Embeded System Lab)
// Engineer: Haojun Xia
// Create Date: 2019/02/08
// Design Name: RISCV-Pipline CPU
// Module Name: ControlUnit
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: RISC-V Instruction Decoder
//////////////////////////////////////////////////////////////////////////////////
`include "Parameters.v"   
module ControlUnit(
    input wire [6:0] Op,
    input wire [2:0] Fn3,
    input wire [6:0] Fn7,
    output wire JalD,
    output wire JalrD,
    output reg [2:0] RegWriteD,
    output wire MemToRegD,
    output reg [3:0] MemWriteD,
    output wire LoadNpcD,
    output reg [1:0] RegReadD,
    output reg [2:0] BranchTypeD,
    output reg [3:0] AluContrlD,
    output wire [1:0] AluSrc2D,
    output wire AluSrc1D,
    output reg [2:0] ImmType        
    );
    
    assign JalD = (Op == 7'b1101111) ? 1'b1 : 1'b0;             //Jal
    assign JalrD = (Op == 7'b1100111) ? 1'b1 : 1'b0;            //Jalr
    assign MemToRegD = (Op == 7'b0000011) ? 1'b1 : 1'b0;        //Load:lb,lbu,lh,lhu,lw
    assign LoadNpcD = JalD | JalrD;
    assign AluSrc1D = (Op == 7'b0010111) ? 1'b1 : 1'b0;
                    //AluSrc1D = 1'b1: AUIPC
    assign AluSrc2D = (Op == 7'b0010011 &&(Fn3 == 3'b001 || Fn3 == 3'b101)) ? 2'b01     //AluSrc2D = 2'b01: SLLI,SRLI,SRAI
                        : ((Op == 7'b0110011 || Op == 7'b1100011) ? 2'b00 : 2'b10);     //AluSrc2D = 2'b00: R-type,Branch
                       
    always @ (*)
    begin
        case(Op)
            7'b0010011:begin 
                            RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b10; BranchTypeD <= `NOBRANCH; ImmType <= `ITYPE;
                            case(Fn3)
                                3'b000: AluContrlD <= `ADD;     //ADDI
                                3'b010: AluContrlD <= `SLT;     //SLTI
                                3'b011: AluContrlD <= `SLTU;    //SLTIU
                                3'b111: AluContrlD <= `AND;     //ANDI
                                3'b110: AluContrlD <= `OR;      //ORI
                                3'b100: AluContrlD <= `XOR;     //XORI
                                3'b001: AluContrlD <= `SLL;     //SLLI
                                3'b101: AluContrlD <= (Fn7 == 7'b0000000) ? `SRL : `SRA;        //SRLI,SRAI
                                default: AluContrlD <= `ADD;
                            endcase
                        end           
            7'b0110111: begin RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b00;        //LUI
                                BranchTypeD <= `NOBRANCH; AluContrlD <= `LUI; ImmType <= `UTYPE; end
            7'b0010111: begin RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b00;        //AUIPC
                                BranchTypeD <= `NOBRANCH; AluContrlD <= `ADD; ImmType <= `UTYPE; end
            7'b0110011: begin 
                            RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b11;        //R-type
                            BranchTypeD <= `NOBRANCH; ImmType <= `RTYPE;
                            case(Fn3)
                                3'b000: AluContrlD <= (Fn7 == 7'b0000000) ? `ADD : `SUB;
                                3'b001: AluContrlD <= `SLL;
                                3'b010: AluContrlD <= `SLT;
                                3'b011: AluContrlD <= `SLTU;
                                3'b100: AluContrlD <= `XOR;
                                3'b101: AluContrlD <= (Fn7 == 7'b0000000) ? `SRL : `SRA;
                                3'b110: AluContrlD <= `OR;
                                3'b111: AluContrlD <= `AND;
                                default: AluContrlD <= `ADD;
                            endcase
                        end
            7'b1101111: begin RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b00;        //Jal
                                BranchTypeD <= `NOBRANCH; AluContrlD <= `ADD; ImmType <= `JTYPE; end
            7'b1100111: begin RegWriteD <= `LW; MemWriteD <= 4'b0000; RegReadD <= 2'b10;        //Jalr
                                BranchTypeD <= `NOBRANCH; AluContrlD <= `ADD; ImmType <= `ITYPE; end
                        //�����ַ��˴���һ��ʼ��ΪRegRead = 2'b00
            7'b1100011: begin 
                            RegWriteD <= `NOREGWRITE; MemWriteD <= 4'b0000; RegReadD <= 2'b11;  //Branch
                            ImmType <= `BTYPE; AluContrlD <= `ADD;          //AluContrlΪ�޹���
                            case(Fn3)
                                3'b000: BranchTypeD <= `BEQ;
                                3'b001: BranchTypeD <= `BNE;
                                3'b100: BranchTypeD <= `BLT;
                                3'b101: BranchTypeD <= `BGE;
                                3'b110: BranchTypeD <= `BLTU;
                                3'b111: BranchTypeD <= `BGEU;
                                default: BranchTypeD <= `BEQ;
                            endcase
                        end
            7'b0000011: begin                                           //Load
                            MemWriteD <= 4'b0000; RegReadD <= 2'b10; BranchTypeD <= `NOBRANCH; 
                            AluContrlD <= `ADD; ImmType <= `ITYPE;
                            case(Fn3)
                                3'b000: RegWriteD <= `LB;
                                3'b001: RegWriteD <= `LH;
                                3'b010: RegWriteD <= `LW;
                                3'b100: RegWriteD <= `LBU;
                                3'b101: RegWriteD <= `LHU;
                                default: RegWriteD <= `LW;
                            endcase
                        end
            7'b0100011: begin                                           //Store
                            RegWriteD <= `NOREGWRITE; RegReadD <= 2'b11; BranchTypeD <= `NOBRANCH;
                            AluContrlD <= `ADD; ImmType <= `STYPE;
                            case(Fn3)
                                3'b000: MemWriteD <= 4'b0001;
                                3'b001: MemWriteD <= 4'b0011;
                                3'b010: MemWriteD <= 4'b1111;
                                default: MemWriteD <= 4'b0000;
                            endcase
                        end
            default: begin RegWriteD <= `NOREGWRITE; MemWriteD <= 4'b0000; RegReadD <= 2'b00;  //Branch
                        BranchTypeD <= `NOBRANCH; ImmType <= `ITYPE; AluContrlD <= `ADD; end
                        //�ȿ�֮һ��һ��ʼBranchTypeD��defaultֵ��Ϊ��BEQ������BranchMakingDecisionģ���Branch�ź�һֱΪ�档
        endcase     
    end
    
endmodule

//功能说明
    //ControlUnit       是本CPU的指令译码器，组合�?�辑电路
//输入
    // Op               是指令的操作码部�?
    // Fn3              是指令的func3部分
    // Fn7              是指令的func7部分
//输出
    // JalD==1          表示Jal指令到达ID译码阶段
    // JalrD==1         表示Jalr指令到达ID译码阶段
    // RegWriteD        表示ID阶段的指令对应的 寄存器写入模�? ，所有模式定义在Parameters.v�?
    // MemToRegD==1     表示ID阶段的指令需要将data memory读取的�?�写入寄存器,
    // MemWriteD        �?4bit，采用独热码格式，对于data memory�?32bit字按byte进行写入,MemWriteD=0001表示只写入最�?1个byte，和xilinx bram的接口类�?
    // LoadNpcD==1      表示将NextPC输出到ResultM
    // RegReadD[1]==1   表示A1对应的寄存器值被使用到了，RegReadD[0]==1表示A2对应的寄存器值被使用到了，用于forward的处�?
    // BranchTypeD      表示不同的分支类型，�?有类型定义在Parameters.v�?
    // AluContrlD       表示不同的ALU计算功能，所有类型定义在Parameters.v�?
    // AluSrc2D         表示Alu输入�?2的�?�择
    // AluSrc1D         表示Alu输入�?1的�?�择
    // ImmType          表示指令的立即数格式，所有类型定义在Parameters.v�?   
//实验要求  
    //实现ControlUnit模块   