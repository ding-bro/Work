// SubaruKwpSystem.cpp: implementation of the CSubaruKwpSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubaruBaseCanApp.h"
#include "Subaru.h"
#include "ErrorCode.h"
#include "EcuInfoCtrl.h"
#include "TroubleCodeCtrl.h"
#include "MessageBoxCtrl.h"
#include "FreezeFrameCtrl.h"
#include "FileManager.h"
#include "arithmetic.h"
#include "MenuCtrl.h"
#include "Expression.h"
#include "DataStreamCtrl.h"
#include "MultiSelectCtrl.h"
#include "ActTestCtrl.h"
#include "InputCtrl.h"
#include "SubaruBaseAllSystemApp.h"
#include "EobdAllSystem.h"
#include "SearchString.h"
//#include "Binary.h"

//extern bool m_bIsService;
//extern W_I32 m_specFuncID;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CBinaryGroup    *g_bgDsId = NULL;

CSubaruBaseCanApp::CSubaruBaseCanApp()
{
    m_pNetLayer = NULL;
}

CSubaruBaseCanApp::~CSubaruBaseCanApp()
{
    if (m_pNetLayer)
    {
        m_pNetLayer->KeepLink(0);
        delete m_pNetLayer;
        m_pNetLayer = NULL;
    }
}

// 安全访问算法
W_ErrorCode CSubaruBaseCanApp::ProtectCal()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    CBinary binAns;
    W_U32 uSeek = 0;
    W_U32 uKey = 0;

    CBinary bin1003("\x10\x03", 2);
    CBinary bin2703("\x27\x03", 2);//sub_49518----sub_23698
    CBinary bin2704("\x27\x04\x00\x00\x00", 6);
    binAns = SubaruCanSendReceive(bin2703);
    if (binAns.GetByteCount() < 4)
    {
        return -1;
    }
    uSeek = binAns[0] * 256 * 256 * 256 + binAns[1] * 256 * 256 + binAns[2] * 256 + binAns[3];
    //uSeek = binAns[0]*256*256*256*256*256 + binAns[1]*256*256*256*256 + binAns[2]*256*256*256 + binAns[3]*256*256+ binAns[4]*256 + binAns[5];
    if (uSeek != 0)
    {
        uKey = SUBARU_Engine_Security_4to4_Bytes(uSeek);
        bin2704.SetByteAt(2, (BYTE)(uKey >> 24) & 0xFF);
        bin2704.SetByteAt(3, (BYTE)(uKey >> 16) & 0xFF);
        bin2704.SetByteAt(4, (BYTE)(uKey >> 8) & 0xFF);
        bin2704.SetByteAt(5, (BYTE)uKey & 0xFF);
        binAns.Clear();
        binAns = SubaruCanSendReceive(bin2704); // bin2703
        if (binAns.GetByteCount() < 2)
            return -1;
    }
    return rect;
}

W_U32 CSubaruBaseCanApp::SUBARU_sub_vident01(W_U32 a1, W_U32 a2)
{
    int v2;
    int v3;
    BYTE byte_10435F8[] = { 0x7,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0x5,0x2,0x7,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0x5,0x2 };
    BYTE byte_10C92FC[] = { 0x5,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0xD,0x2,0xB,0xF,0x4,0x0,0x3,0xB,0x4,0x6,0x0,0xF,0x2,0xD,0x9,0x5,0xC,0x1,0xA,0x3,0xD,0xE,0x8 };
    BYTE byte_10C32FC[] = { 0x7,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0x5,0x2 };
    BYTE byte_10437F7[] = { 0x3,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0x5,0x2,0x7,0x6,0x7,0x1,0x9,0xC,0xD,0x8,0xA,0x5,0x2,0xD,0x8,0xA,0x5,0x2,0x7,0x6,0x7 };
    BYTE byte_1022016[] = { 0x4,0x6,0x7,0x1,0x9,0xC,0x3,0x8,0xA,0x5,0x2,0x7,0x6,0x7,0x1,0x9,0xC,0xD,0x6,0xA,0x5,0x2,0xD,0x8,0x7,0x5,0x2,0x7,0x6,0x7 };

    v2 = (a1 ^ a2) << 16;

    W_U32 t1 = (W_U32)v2 >> 28;
    W_U32 t2 = (W_U32)v2 >> 16;
    t2 = t2 & 0x01;
    W_U32 t3 = (v2 >> 24) & 0x1F;
    W_U32 t4 = (v2 >> 20) & 0x1F;
    W_U32 t5 = ((W_U32)v2 >> 16) & 0x1F;

    v3 = (16 * (16 * (16 * byte_10C92FC[(t1) | 16 * (t2)] | byte_10C92FC[t3]) | byte_10C92FC[t4]) | byte_10C92FC[t5]) & 0xFFFF;
    return ((v3 >> 3) | (v3 << 13)) & 0xFFFF;
}

W_U32 CSubaruBaseCanApp::SUBARU_Engine_Security_4to4_Bytes(W_U32 a1)
{
    int v1;
    int v2;
    W_U16 v3;
    W_U16 v4;
    int v5;
    W_U16 v7;
    W_U16 v8;
    W_U16 dest[16] = { 0x862B, 0x3ECC, 0x8961, 0x1895, 0x93F9, 0x4B75, 0x7F4A, 0xB046,
        0x5E71, 0xFD21, 0x35F4, 0xAD6B, 0x9EA5, 0x201C, 0x4625, 0x78B1 };

    v1 = a1;
    v7 = ((v1 >> 8) & 0x0000FF) * 256 + ((v1 >> 24) & 0xFF);
    v8 = ((v1 >> 16) & 0x00FF) * 256 + (v1 & 0xFF);

    v2 = 0;
    do
    {
        v3 = SUBARU_sub_vident01(
            v7, dest[v2]);
        v4 = v7;
        v2++;
        v7 = v3 ^ v8;
        v8 = v4;
    } while (v2 != 16);

    v5 = ((v7 >> 8) & 0xFF) * 0x1000000 + ((v4 >> 8) & 0xFF) * 0x10000 + (v7 & 0xFF) * 0x100 + (v4 & 0xFF);
    return v5;
}

W_ErrorCode CSubaruBaseCanApp::SendAndRecive(const CBinary SendCmd, CBinary& binRecive)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    binRecive = SubaruCanSendReceive(SendCmd);
    if (0 >= binRecive.GetByteCount())
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    else if (0x7f == binRecive[0])
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }

    return rect;
}

// 进系统后的所有询问命令发送和保存回复
W_ErrorCode    CSubaruBaseCanApp::SendAndSaveEnterCmd()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;

    CBinary binTemp, binAns;
    W_I16    NeedCmdNum, startPos, CmpValue, ALLposNum, Pos0Num;
    bool    SendFlag = true;
    CSearchString SubaMenuComd;
    vector<string>    vctStr;
    vector<vector<string>> SearchResault;
    CBinary    bin22f197("\x22\xf1\x97", 3);
    CBinary    bin22f182("\x22\xf1\x82", 3);
    CBinary bin221000("\x22\x10\x00", 3);    //回复了此命令的系统会根据回复发送221001 --- 22100A
    W_I16    cmp221000[2] = { 0, 0 };

    EnterRecSet.clear();
    if (FALSE == SubaMenuComd.OpenTabFile(Subaru_MenuComd_CBF))   //进入命令集合
        return CErrorCode::EC_DATA;

    vctStr.push_back(SystemNum);  // SystemNum在 EnterSystem()函数 中进行筛选
    SubaMenuComd.SearchString(SearchResault, FALSE, 0, 0, vctStr);  // 以 SystemNum 在 SubaMenuComd 中搜索
    if (SearchResault.size() == 0)//有些系统没有这些命令
        return rect;
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        SendFlag = true;
        binAns.Clear();
        binTemp = String2Binary(SearchResault[i][3]);  //  binTemp = SubaMenuComd 第3列
        if (0x00 == StringToHex(SearchResault[i][3]))
            break;
        if (0 == i && 3 == binTemp.GetByteCount())    // i == 0 && SubaMenuComd[3] == 3
            startPos = 3;
        else if (0 == i)
            startPos = 2;

        NeedCmdNum = StringToHex(SearchResault[i][1].c_str());  // NeedCmdNum = SubaMenuComd 第1列
        CmpValue = StringToHex(SearchResault[i][2].c_str());  // CmpValue = SubaMenuComd 第2列
        ALLposNum = StringToHex(SearchResault[i][4].c_str());  // ALLposNum =  SubaMenuComd 第4列
        Pos0Num = StringToHex(SearchResault[i][5].c_str()); // Pos0Num = SubaMenuComd 第5列
        if (0 != NeedCmdNum)   // NeedCmdNum = 0 ，必发命令
        {
            SendFlag = false;
            for (map<W_I16, W_I16>::iterator pMap = EnterRecSet.begin(); pMap != EnterRecSet.end(); pMap++)
            {
                if (pMap->first == NeedCmdNum)  // 如果存放恢复数据map的key值 == NeedCmdNum(上一条命令的最后一位)
                {   // ->->->->->->->->
                    if (pMap->second & CmpValue)   // 每4位为一组，存储的数据的最后一位 & SubaMenuComd[2] 为真的话，SendFlag置为true
                        SendFlag = true;
                    break;
                }
            }
        }
        else if (0 == Pos0Num && 1 == SubaruBRZ_SET->VehicleStatus && 0 != EnterRecSet.size())
        {
            SendFlag = false;
        }
        if (SendFlag)   // 当SendFlag为true时，发命令
            binAns = SubaruCanSendReceive(binTemp);
        if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7f)
            binAns.Clear();
        // ->->->->->->->   Pos0Num为key值  回复数据为value值
        for (W_I16 j = 0; j < ALLposNum && j + startPos < binAns.GetByteCount(); j++)  // 将上一条回复命令的[3] ~ [6]位存储起来
            EnterRecSet[Pos0Num + j] = binAns[j + startPos];    // 将每条回复的数据的最后4位为一组存储起来
    }

    if (1 == SubaruBRZ_SET->VehicleStatus)
    {
        binAns = SubaruCanSendReceive(bin22f182);    //尚不清楚命令作用

        //询问系统名字
        binAns = SubaruCanSendReceive(bin22f197);
        if (binAns.GetByteCount() > 0 && binAns[0] != 0x7f)
        {
            SystemName = "";
            for (W_I16 i = 3; i < binAns.GetByteCount(); i++)
                SystemName += binAns[i];
        }

        binAns = SubaruCanSendReceive(bin221000);
        if (binAns.GetByteCount() > 3 && 0x7f != binAns[0])
            cmp221000[0] = binAns[3];
        if (binAns.GetByteCount() > 4 && 0x7f != binAns[0])
            cmp221000[1] = binAns[4];
        for (W_I16 i = 0; i < 8; i++)
        {
            if ((0x80 >> i)& cmp221000[0])
            {
                binTemp = bin221000;
                binTemp.SetByteAt(2, i + 1);
                binAns = SubaruCanSendReceive(binTemp);
            }
        }
        if (0x80 & cmp221000[1])
        {
            binTemp = bin221000;
            binTemp.SetByteAt(2, 0x09);
            binAns = SubaruCanSendReceive(binTemp);
        }
        if (0x40 & cmp221000[1])
        {
            binTemp = bin221000;
            binTemp.SetByteAt(2, 0x0A);
            binAns = SubaruCanSendReceive(binTemp);
        }
        if (binAns.GetByteCount() <= 0 || 0x7f == binAns[0])
        {
            if (Auto_Scan != m_AutoScan)
            {
                EnterRecSet.clear();
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                rect = CErrorCode::EC_COMMUNICATION;
            }
        }
    }

    return rect;
}

// DYN
W_ErrorCode CSubaruBaseCanApp::New_EnterSystem()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CReceiveFrame    rfRet;
    CSearchString    SubaGotoFile;
    vector<string>    vctStr;
    vector<vector<string>> SearchResault;

    SetMode(Suba_Can);  // Suba_Can
    rfRet = InitComm(Suba_Can);    // 初始化CAN通信     发命令①
    if (!rfRet.GetByteCount())
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    AnsforEnter = SubaruCanSendReceive(SubaruBRZ_SET->EnterCMD);   // 发命令②
    if (AnsforEnter.GetByteCount() < 5 || 0x7f == AnsforEnter[0])
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (SubaruBRZ_SET->EnterCMD.GetByteCount() == 1)
        AnsforEnter.DeleteByte(0);
    else
    {
        AnsforEnter.DeleteByte(0);
        AnsforEnter.DeleteByte(0);
        if (SubaruBRZ_SET->EnterCMD.GetByteCount() != 2)
            AnsforEnter.DeleteByte(0);
    }
    for (W_I16 i = 3; i < AnsforEnter.GetByteCount(); )
        AnsforEnter.DeleteByte(i);

    SubaruBRZ_SET->SysFlag = true;


    //CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);     // 命令
    //// 发送询问命令,将回复编码
    //if (CErrorCode::EC_SUCCESS != (rect = SendAndSaveEnterCmd()))
    //{
    //    CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    //    return CErrorCode::EC_COMMUNICATION;
    //}

    if (1 == SubaruBRZ_SET->VehicleStatus && Auto_Scan != m_AutoScan)
        m_pNetLayer->KeepLink(2000, CBinary("\x3E\x00", 2));

    return rect;
}



W_ErrorCode CSubaruBaseCanApp::EnterSystem()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CReceiveFrame    rfRet;
    CSearchString    SubaGotoFile;
    vector<string>    vctStr;
    vector<vector<string>> SearchResault;

    SetMode(Suba_Can);  // Suba_Can
    rfRet = InitComm(Suba_Can);    // 初始化CAN通信     发命令①
    if (!rfRet.GetByteCount())
    {
        CErrorCode::SetLastError(CErrorCode::EC_COMMUNICATION);
        return CErrorCode::EC_COMMUNICATION;
    }

    AnsforEnter = SubaruCanSendReceive(SubaruBRZ_SET->EnterCMD);   // 发命令②
    if (AnsforEnter.GetByteCount() < 5 || 0x7f == AnsforEnter[0])
    {
        CErrorCode::SetLastError(CErrorCode::EC_COMMUNICATION);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (SubaruBRZ_SET->EnterCMD.GetByteCount() == 1)
        AnsforEnter.DeleteByte(0);
    else
    {
        AnsforEnter.DeleteByte(0);
        AnsforEnter.DeleteByte(0);
        if (SubaruBRZ_SET->EnterCMD.GetByteCount() != 2)
            AnsforEnter.DeleteByte(0);
    }
    for (W_I16 i = 3; i < AnsforEnter.GetByteCount(); )
        AnsforEnter.DeleteByte(i);

    SubaruBRZ_SET->SysFlag = true;
    //得到系统编号
    if (FALSE == SubaGotoFile.OpenTabFile(Subaru_GotoFile_CBF))  //进入命令取值区间(部分系统没有遵循)
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }
    vctStr.push_back(SubaruBRZ_SET->SubaDLLPrefix);  // 在 SubaCANProInto()函数 中进行筛选
    SubaGotoFile.SearchString(SearchResault, FALSE, 0, 0, vctStr);  // 以 SubaDLLPrefix 在 SubaGotoFile 的第0列搜索
    if (SearchResault.size() <= 0)
    {
        SubaGotoFile.SearchString(SearchResault, FALSE, 1, 1, vctStr);  // 如果取出来 SubaDLLPrefix 第0列没有，则在SubaGotoFile的第一列搜索
        if (SearchResault.size() <= 0)
        {
            CErrorCode::SetLastError(CErrorCode::EC_DATA);
            return CErrorCode::EC_DATA;
        }
    }
    string TempSysNum = "";
    for (W_I16 i = 0; i < SearchResault.size(); i++)    // 筛选 SystemNum
    {
        SystemNum = SearchResault[i][2];  // SystemNum = SubaGotoFile中的第2列
        if (SearchResault.size() > 1)
        {
            if (AnsforEnter.GetByteCount() != 3)
            {
                if (!m_AutoScan)
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
                return CErrorCode::EC_SUCCESS;
            }
            if (SubaruBRZ_SET->SubaDLLPrefix == "00019" || SubaruBRZ_SET->SubaDLLPrefix == "00016")
            {
                if ((AnsforEnter.Compare(String2Binary(SearchResault[i][4])) >= 0 && AnsforEnter.Compare(String2Binary(SearchResault[i][5])) <= 0))
                    break;
            }
            else
            {
                if ("000000" == SearchResault[i][4] && "ffffff" == SearchResault[i][5])
                    break;//TempSysNum = SystemNum;                 // 只有符合条件才能取 SystemNum
                else if ((AnsforEnter.Compare(String2Binary(SearchResault[i][4])) >= 0 && AnsforEnter.Compare(String2Binary(SearchResault[i][5])) <= 0)
                    && (SubaruBRZ_SET->SubaDLLPrefix_GoFile == SearchResault[i][3]))   // EnterCMD 回复的数据要大于 SubaGotoFile[4] 并且要小于 SubaGotoFile[5]
                    break;    // 在SubaCANProInto[1] 中取出的 SubaDLLPrefix_GoFile == SubaGotoFile[3]  (取SystemNum的搜索范围)
            }
            SystemNum = "";
        }
    }
    if (SystemNum.length() <= 0)
    {
        if (1 != SubaruBRZ_SET->VehicleStatus && SearchResault.size() > 1)
        {
            SystemNum = SearchResault[0][2];
        }
        else if (TempSysNum.length() > 0)
        {
            SystemNum = TempSysNum;
        }
        else
        {
            if (!m_AutoScan)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
            return CErrorCode::EC_DATA;
        }
    }
    if ("00000007" == SystemNum)//if ("DBFTA000" == SystemNum)
    {
        // BRZ车型 安全气囊系统 安全进入
        CBinary bin2701("\x27\x01\x4b\x2d", 4);
        CBinary bin2702("\x27\x02", 2);
        CBinary binAns;
        W_I16 BYTE0 = 0x145;
        W_I16 BYTE1 = 0x143;
        binAns = SubaruCanSendReceive(bin2701);
        if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7f)
        {
            if (!m_AutoScan)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
            return CErrorCode::EC_DATA;
        }
        if (binAns.GetByteCount() > 2)
        {
            BYTE0 = BYTE0 - binAns[2];
            if (binAns.GetByteCount() > 3)
                BYTE1 = BYTE1 - binAns[3];
            if (BYTE1 < 0x100)
                BYTE0 -= 1;

            bin2702 += (BYTE0 & 0xff);
            bin2702 += (BYTE1 & 0xff);
            binAns = SubaruCanSendReceive(bin2702);
            if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7f)
            {
                if (!m_AutoScan)
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
                return CErrorCode::EC_DATA;
            }
        }
    }

    CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    // 发送询问命令,将回复编码
    if (CErrorCode::EC_SUCCESS != (rect = SendAndSaveEnterCmd()))
    {
        CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (1 == SubaruBRZ_SET->VehicleStatus && Auto_Scan != m_AutoScan)
        m_pNetLayer->KeepLink(2000, CBinary("\x3E\x00", 2));

    return rect;
}

//筛选菜单
W_ErrorCode    CSubaruBaseCanApp::GetMenu(map<W_I16, W_I16>& MenuNum)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CSearchString SubaMenuShow;
    vector<string> vctStr, vecCmd;
    vector<vector<string>> SearchResault;
    W_U8 iFlag = 0;
    W_I16 MenuCode = 0;

    MenuShow.clear();
    MenuNum.clear();
    ReadTroubleMode.clear();
    ECUInfo.clear();
    DsSet.clear();
    SpecfunsSet.clear();
    GetDS.clear();

    if (FALSE == SubaMenuShow.OpenTabFile(Subaru_MenuShow_CBF))  //功能菜单
        return CErrorCode::EC_DATA;

    /* 查找过滤不符合条件的菜单项 */
    vctStr.push_back(SystemNum);  // 插入 SystemNum = SubaGotoFile[2]
    SubaMenuShow.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (vctStr.size() < 10)
            continue;

        iFlag = 0;
        if (vctStr[1] == "FF")    // 判断 SubaMenuShow 中的第1列数据
        {
            iFlag = 1;
        }
        else if (vctStr[1] == "00")
        {
            for (map<W_I16, W_I16>::iterator pMap = EnterRecSet.begin(); pMap != EnterRecSet.end(); pMap++)
            {
                if (pMap->first == StringToHex(vctStr[2]))    // SubaMenuShow[2] == 存放回复数据map的key值
                {
                    if (pMap->second & StringToHex(vctStr[3]))    // SubaMenuShow[3] & 存放回复数据map的value值
                        iFlag = 1;
                    break;
                }
            }
        }
        else if (vctStr[1] == "F0")
        {
            iFlag = 1;
            for (map<W_I16, W_I16>::iterator pMap = EnterRecSet.begin(); pMap != EnterRecSet.end(); pMap++)
            {
                if (pMap->first == StringToHex(vctStr[2]))
                {
                    if (pMap->second & StringToHex(vctStr[3]))
                        iFlag = 0;
                    break;
                }
            }
        }
        if (1 == iFlag)   // iFlag 为1存入项
        {
            /* 在文件GotoMenu里有一项条件,因为只有一个系统真正需要使用这个,所以直接写在代码里 */
            //if ("00000087" != SystemNum || "60351620" != vctStr[5])//if ("DBFUO000" != SystemNum || "00003262" != vctStr[5])
            //    MenuShow.push_back(vctStr);
            //else if (CBinary("\x00\x00\x01", 3) == AnsforEnter && "02871620" == vctStr[4])//else if (CBinary("\x00\x00\x01", 3) == AnsforEnter && "00008000" == vctStr[4])
            //    MenuShow.push_back(vctStr);
            //else if ("fcf81620" == vctStr[4])//else if ("00008001" == vctStr[4])
            MenuShow.push_back(vctStr);  // 将筛选好的数据插入 MenuShow 中
        }
    }

    vecCmd.clear();
    /* 读故障码 清故障码 */
    for (W_I16 i = 0; i < MenuShow.size(); i++)  // 遍历筛选好的数据
    {
        iFlag = 0;
        vctStr = MenuShow[i];
        if (vctStr[7] == "e8021620" && MenuNum.size() <= 0 && SystemNum != "00000075")//if (vctStr[7] == "000005ea" && MenuNum.size() <= 0)
        {                                                // 根据 MenuShow 第7列判断是否为 "e8021620"
            vecCmd.push_back(vctStr[7]);
            iFlag = 1;
            MenuNum[MenuCode++] = 0;  // 根据 "e8021620" 的数量，map容器插入 0 和 1
            MenuNum[MenuCode++] = 1;
        }
        // iFlag
        for (W_I16 j = i + 1; j < MenuShow.size() && 1 == iFlag; j++)  // 满足vctStr[7] == "e8021620" && MenuNum.size() <= 0 && SystemNum != "00000075才会执行
        {
            if (MenuShow[j][5] == vctStr[7])     // 在SystemNum搜索范围下，如果[5] = e8021620
            {
                ReadTroubleMode.push_back(MenuShow[j][6]);   // 符合条件的插入[6]，存入读码模式中，为下方读码做准备
            }
        }
        /*for (W_I16 j = 0; j < vecCmd.size(); j++)
        {
            if (vecCmd[j] == vctStr[5])
            {
                iFlag = 1;
                break;
            }
        }
        for (W_I16 j = i + 1; j < MenuShow.size() && 1 == iFlag; j++)
        {
            if (MenuShow[j][5] == vctStr[7])
            {
                vecCmd.push_back(vctStr[7]);
                break;
            }
            else if (j == MenuShow.size() - 1 && MenuShow[j][5] != vctStr[7])
            {
                ReadTroubleMode.push_back(vctStr[6]);
            }
        }*/
    }

    if (Auto_Scan == m_AutoScan)
        return rect;

    /* 读数据流 */
    for (W_I16 i = 0; i < MenuShow.size(); i++)   // 判断是否要出现数据流功能
    {
        vctStr = MenuShow[i];
        if ("eb021620" == vctStr[7]) //if ("000005e9" == vctStr[7])
        {
            MenuNum[MenuCode++] = 2;   // 插入2
            CSearchString SubaData;
            if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))  //数据流库
                return CErrorCode::EC_DATA;
            vctStr.clear();
            vctStr.push_back(SystemNum);
            SubaData.SearchString(GetDS, FALSE, 0, 0, vctStr);    // 以系统编号搜索,存入GetDS
            break;
        }
    }
    CSearchString datadefvalue;
    if (FALSE == datadefvalue.OpenTabFile(Subaru_DataDefValue_CBF))  //部分数据流条件命令编号
        return CErrorCode::EC_DATA;
    vctStr.clear();
    vctStr.push_back(SystemNum);
    SearchResault.clear();
    datadefvalue.SearchString(SearchResault, FALSE, 0, 0, vctStr);  // 以系统编号搜索
    W_I16 ShortTemp;
    W_I16 ShortTemp1;
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        ShortTemp = StringToHex(SearchResault[i][1]);
        ShortTemp1 = StringToHex(SearchResault[i][2]);
        EnterRecSet[ShortTemp] |= ShortTemp1;
    }
    /* 筛选数据流 第一次筛选(根据进入命令,部分系统有特殊过滤) */
    SearchResault.clear();    //作为GetDSTemp
    for (W_I16 i = 0; i < GetDS.size(); i++)
    {
        if (GetDS[i].size() < 20 || "00002001" == GetDS[i][GetDS[i].size() - 1]  // 最后一项
            || "00002002" == GetDS[i][GetDS[i].size() - 1] || "00002003" == GetDS[i][GetDS[i].size() - 1])
            continue;
        if ("00000007" == GetDS[i][0] && "00000001" != GetDS[i][GetDS[i].size() - 1])//if ("DBFTA000" == GetDS[i][0] && "00000001" != GetDS[i][GetDS[i].size() - 1])
            continue;

        iFlag = 0;
        ShortTemp = StringToHex(GetDS[i][3]);
        ShortTemp1 = StringToHex(GetDS[i][4]);
        for (map<W_I16, W_I16>::iterator pMap = EnterRecSet.begin(); pMap != EnterRecSet.end(); pMap++)
        {
            if (pMap->first == ShortTemp && (ShortTemp1 & pMap->second))  // Subaru_Data[3] == 回复数据key值  &&  Subaru_Data[4] & 回复数据value值
            {
                iFlag = 1;    // 筛选数据流发送命令
                break;
            }
        }
        if (SystemNum == "00000083") //驻车制动系统只发一条进入命令，没有筛选命令
            iFlag = 1;
        if (iFlag)
            SearchResault.push_back(GetDS[i]);
    }
    GetDS.clear();
    GetDS = SearchResault;

    /* BRZ车型没有特殊功能 */
    if (2 == SubaruBRZ_SET->VehicleStatus)
    {
        if (SystemNum == "00000010")
            MenuNum[MenuCode++] = 0x0f;
        return rect;
    }

    /* ECU规格信息 只有该系统有特殊功能，才会有ECU规格信息*/
    for (W_I16 i = 0; i < MenuShow.size(); i++)
    {
        if ("7a241620" == MenuShow[i][5] && "55351620" == MenuShow[i][7])//if ("00003257" == MenuShow[i][5])
        {
            CFileManager profile;
            CBinary         binSystemNum = HexString2Binary(SystemNum);
            CBinary         binSearchID;
            binSearchID = CBinary("\x00", 1);
            binSearchID += binSystemNum[3];
            binSearchID += String2Binary(MenuShow[i][6]);
            string         strData = GetStringValue(Subaru_SubaSpfunction_File, binSearchID);   //帅选特殊功能的显示菜单
            if (!strData.size())
                return rect = CErrorCode::EC_DATA;
            profile.InitManager(strData);
            string         strMode = profile.GetItemValue("NAME", "Mode");
            if (strMode != "00")
            {
                for (W_I16 j = 0; j < MenuShow.size(); j++)
                {
                    if ("55351620" == MenuShow[j][5])//if ("00003257" == MenuShow[i][5])
                    {
                        ECUInfo.push_back(MenuShow[j]);
                    }
                }
            }

            break;
        }
    }
    if (ECUInfo.size() > 0)
        MenuNum[MenuCode++] = 0x03;

#ifdef    __NT5600DS__
    return rect;
#endif
    vector<vector<string>> menuTemp;
    vector<vector<string>> IDTemp;
    /* 特殊功能 */
    for (W_I16 i = 0; i < MenuShow.size(); i++)
    {
        if ("7a241620" == MenuShow[i][7] || "651f1620" == MenuShow[i][7])//if ("00002378" == MenuShow[i][7])651f1620为驻车制动系统独有的
        {
            vctStr.clear();
            vctStr.push_back(MenuShow[i][7]);
            vctStr.push_back(MenuShow[i][6]);
            IDTemp.push_back(vctStr);
            /* 将所有特殊功能保存为 特殊功能->路径2->路径3->...特殊功能名->特殊功能ID */
            for (W_I16 j = i; j < MenuShow.size(); j++)
            {
                for (W_I16 k = 0; k < IDTemp.size(); k++)
                {
                    if (CompareStringIsEquation(MenuShow[j][5], IDTemp[k][IDTemp[k].size() - 2]))  // MenuShow[5] = IDTemp[k]
                    {
                        IDTemp[k].erase(IDTemp[k].end() - 1);
                        menuTemp.push_back(IDTemp[k]);
                        IDTemp.erase(IDTemp.begin() + k--);
                        break;
                    }
                }
                for (W_I16 k = 0; k < menuTemp.size(); k++)
                {
                    if (CompareStringIsEquation(MenuShow[j][5], menuTemp[k][menuTemp[k].size() - 1]))
                    {
                        vctStr.clear();
                        vctStr = menuTemp[k];
                        vctStr.push_back(MenuShow[j][7]);
                        vctStr.push_back(MenuShow[j][6]);
                        IDTemp.push_back(vctStr);
                        break;
                    }
                }
            }
            if (SystemNum == "00000029" && MenuShow[i + 1][6] == "12637620")
            {
                vctStr.clear();
                vctStr.push_back("7a241620");
                vctStr.push_back(MenuShow[i + 1][7]);
                vctStr.push_back(MenuShow[i + 1][6]);
                IDTemp.push_back(vctStr);
            }
            break;
        }
    }
    /* 加载已添加代码的特殊功能任务ID */
    SpecFunsIDSet();
    /* 筛选出已有代码的ID */
    for (W_I16 i = 0; i < IDTemp.size(); i++)
    {
        for (W_I16 j = 0; j < SpecFunsID.size(); j++)
        {
            if (CompareStringIsEquation(IDTemp[i][IDTemp[i].size() - 1], SpecFunsID[j]))
            {
                SpecfunsSet.push_back(IDTemp[i]);
                break;
            }
        }
    }
    if (SpecfunsSet.size() > 0)
        MenuNum[MenuCode++] = 0x0f;

    return rect;
}

W_I16 CSubaruBaseCanApp::New_MenuAndProcess()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CBinary            binMenu;

    if (m_bIsService)
    {
        if (m_specFuncID == 0)
            return 0;
        else
        {
            switch (m_specFuncID)
            {
            case 0x028f4f20: {binMenu = HexString2Binary("53200000003C"); rect = SpecFunsID_028f4f20(binMenu); break; }        // 更换DPF                              
            case 0x028e4f20: {binMenu = HexString2Binary("53200000003D"); rect = SpecFunsID_028e4f20(binMenu); break; }        // 更新DPF                              
            case 0x028d4f20: {binMenu = HexString2Binary("532000000042"); rect = SpecFunsID_028d4f20(binMenu); break; }        //更换机油                              
            case 0x028c4f20: {binMenu = HexString2Binary("530000010182"); rect = SpecFunsID_028c4f20(binMenu); break; }        //读取DPF／机油相关的训练值。ECM -> TOOL
            case 0x028b4f20: {binMenu = HexString2Binary("530000010186"); rect = SpecFunsID_028b4f20(binMenu); break; }        //写入DPF／机油相关的训练值。TOOL -> ECM
            case 0x02674f20: {binMenu = HexString2Binary("534d62111620"); rect = SpecFunsID_02674f20(binMenu); break; }        //新注册喷油器代码（TOOL至ECM）         
            case 0x02664f20: {binMenu = HexString2Binary("534d63111620"); rect = SpecFunsID_02664f20(binMenu); break; }        //读取喷油器代码（ECM至TOOL）           
            case 0x02644f20: {binMenu = HexString2Binary("532000000051"); rect = SpecFunsID_02644f20(binMenu); break; }        //喷油器代码显示                        
            case 0x02071a20: {binMenu = HexString2Binary("53200000005D"); rect = SpecFunsID_02071a20(binMenu); break; }        //防盗锁系统                                
            case 0x02764f20: {binMenu = HexString2Binary("532000000087"); rect = SpecFunsID_02764f20(binMenu); break; }        //燃油喷射器喷射量记忆                  
            case 0x02061b20: {binMenu = HexString2Binary("53200002001e"); rect = SpecFunsID_02061b20(binMenu); break; }        //VSC（VDC）中间值设置模式
            case 0x02021b20: {binMenu = HexString2Binary("534dcf761620"); rect = SpecFunsID_02021b20(binMenu); break; }        //刹车维护模式
            case 0x32667620: {binMenu = HexString2Binary("53200002002b"); rect = SpecFunsID_32667620(binMenu); break; }        //擦除无钥ID码
            case 0x42667620: {binMenu = HexString2Binary("53200002002a"); rect = SpecFunsID_42667620(binMenu); break; }        //无钥匙 ID 注册
            }
            if (rect != CErrorCode::EC_SUCCESS)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return 0;
        }
    }

    map<W_I16, W_I16> MenuNum;
    DataStatus.clear();

    CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    //rect = GetMenu(MenuNum);
    //if (CErrorCode::EC_SUCCESS != rect)
    //    return rect;
    //if (0 == MenuNum.size())    //532000000006        "系统不支持"
    //{
    //    if (Auto_Scan != m_AutoScan)
    //        FxShowMessageBox(STD_TTL_MSG_INFORMATION, CBinary("\x53\x20\x00\x00\x00\x06", 6), DF_MB_OK, DT_LEFT);
    //    return rect;
    //}

    //if (SystemName.length() > 0 && Auto_Scan != m_AutoScan)
    //{
    //    /* 显示系统名 */
    //    for (W_I16 i = SystemName.length() - 1; i >= 0; i--)
    //    {
    //        if (0x20 == SystemName.at(i) || 0x7f == SystemName.at(i))
    //            SystemName.erase(SystemName.begin() + i);
    //        else
    //            break;
    //    }
    //    FxShowMessageBox(FxGetStdString(STD_TTL_MSG_INFORMATION), SystemName, DF_MB_OK, DT_CENTER);
    //}

    W_I16 iSelect = 0;
    CMenuCtrl iMenu;
    while (1)
    {
        //for (W_I16 i = 0; i < MenuNum.size(); i++)
        //{
        //    if (0x00 == MenuNum[i])
        //        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x01", 6));    // 读码
        //    else if (0x01 == MenuNum[i])
        //        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x02", 6));    // 清码
        //    else if (0x02 == MenuNum[i])
        //        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x03", 6));    // 读数据流
        //    else if (0x03 == MenuNum[i])
        //        iMenu.AddOneItem(CBinary("\x53\x20\x00\x01\x00\x02", 6));    // ECU规格信息
        //    else if (0x0f == MenuNum[i])
        //        iMenu.AddOneItem(STD_INFO_SPECIAL_FUNCTIONS);                // 特殊功能
        //}

        iMenu.InitCtrl(CBinary("\x53\x00\x00\x00\x02\x00", 6));
        iMenu.AddOneItem(CBinary("\x53\x20\x00\x01\x00\x02", 6));    // ECU规格信息
        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x01", 6));    // 读故障码
        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x02", 6));   // 清故障码
        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x03", 6));    // 读数据流
        iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x13", 6));    // 动作测试
        iMenu.AddOneItem(STD_INFO_SPECIAL_FUNCTIONS);                // 特殊功能


        if (Auto_Scan != m_AutoScan)
            iSelect = iMenu.ShowCtrl();
        else if (0xff == m_quick_Erase)
            return QuickClearTroubleCode();
        else
            return ReadTroubleCode();
        if (-1 == iSelect)
        {
            CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
#ifdef    __NT5600DPF__
            if (FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_EXITSYSTEM, DF_MB_YESNO) == DF_IDYES)
#endif
                break;
            continue;
        }

        switch (iSelect)
        {
        case 0x00:    // 读ECU规格信息
            //rect = ReadEcuInfo();
            rect = Read_computer_info();
            break;
        case 0x01:    // 故障码
            rect = New_ReadTroubleCode();
            if (CErrorCode::EC_ECU_REFUSE == rect)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_THE_SELECTED_FUNCTION_IS_NOT_SUPPORTED, DF_MB_OK, DT_LEFT);
            else
                New_SubaruCanDtcShow();
            break;
        case 0x02:  // 清故障码
            rect = New_ClearTroubleCode();
            break;
        case 0x03:    // 数据流
            //rect = DataStream();
            rect = New_DataStream();
            break;
        case 0x04:    // 动作测试
             //rect = ReadEcuInfo();
            rect = Act_Test();
            break;
        case 0x05:    // 特殊功能
            rect = SpecialFunction();
            break;

        }
    }
    return rect;
}


W_I16 CSubaruBaseCanApp::MenuAndProcess()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CBinary            binMenu;

    if (m_bIsService)
    {
        if (m_specFuncID == 0)
            return 0;
        else
        {
            switch (m_specFuncID)
            {
            case 0x028f4f20: {binMenu = HexString2Binary("53200000003C"); rect = SpecFunsID_028f4f20(binMenu); break; }        // 更换DPF                              
            case 0x028e4f20: {binMenu = HexString2Binary("53200000003D"); rect = SpecFunsID_028e4f20(binMenu); break; }        // 更新DPF                              
            case 0x028d4f20: {binMenu = HexString2Binary("532000000042"); rect = SpecFunsID_028d4f20(binMenu); break; }        //更换机油                              
            case 0x028c4f20: {binMenu = HexString2Binary("530000010182"); rect = SpecFunsID_028c4f20(binMenu); break; }        //读取DPF／机油相关的训练值。ECM -> TOOL
            case 0x028b4f20: {binMenu = HexString2Binary("530000010186"); rect = SpecFunsID_028b4f20(binMenu); break; }        //写入DPF／机油相关的训练值。TOOL -> ECM
            case 0x02674f20: {binMenu = HexString2Binary("534d62111620"); rect = SpecFunsID_02674f20(binMenu); break; }        //新注册喷油器代码（TOOL至ECM）         
            case 0x02664f20: {binMenu = HexString2Binary("534d63111620"); rect = SpecFunsID_02664f20(binMenu); break; }        //读取喷油器代码（ECM至TOOL）           
            case 0x02644f20: {binMenu = HexString2Binary("532000000051"); rect = SpecFunsID_02644f20(binMenu); break; }        //喷油器代码显示                        
            case 0x02071a20: {binMenu = HexString2Binary("53200000005D"); rect = SpecFunsID_02071a20(binMenu); break; }        //防盗锁系统                                
            case 0x02764f20: {binMenu = HexString2Binary("532000000087"); rect = SpecFunsID_02764f20(binMenu); break; }        //燃油喷射器喷射量记忆                  
            case 0x02061b20: {binMenu = HexString2Binary("53200002001e"); rect = SpecFunsID_02061b20(binMenu); break; }        //VSC（VDC）中间值设置模式
            case 0x02021b20: {binMenu = HexString2Binary("534dcf761620"); rect = SpecFunsID_02021b20(binMenu); break; }        //刹车维护模式
            case 0x32667620: {binMenu = HexString2Binary("53200002002b"); rect = SpecFunsID_32667620(binMenu); break; }        //擦除无钥ID码
            case 0x42667620: {binMenu = HexString2Binary("53200002002a"); rect = SpecFunsID_42667620(binMenu); break; }        //无钥匙 ID 注册
            }
            if (rect != CErrorCode::EC_SUCCESS)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return 0;
        }
    }

    map<W_I16, W_I16> MenuNum;
    DataStatus.clear();

    CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    rect = GetMenu(MenuNum);
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;
    if (0 == MenuNum.size())    //532000000006        "系统不支持"
    {
        if (Auto_Scan != m_AutoScan)
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, CBinary("\x53\x20\x00\x00\x00\x06", 6), DF_MB_OK, DT_LEFT);
        return rect;
    }
    if (SystemName.length() > 0 && Auto_Scan != m_AutoScan)
    {
        /* 显示系统名 */
        for (W_I16 i = SystemName.length() - 1; i >= 0; i--)
        {
            if (0x20 == SystemName.at(i) || 0x7f == SystemName.at(i))
                SystemName.erase(SystemName.begin() + i);
            else
                break;
        }
        FxShowMessageBox(FxGetStdString(STD_TTL_MSG_INFORMATION), SystemName, DF_MB_OK, DT_CENTER);
    }

    W_I16 iSelect = 0;
    CMenuCtrl iMenu;
    while (1)
    {
        iMenu.InitCtrl(CBinary("\x53\x00\x00\x00\x02\x00", 6));
        for (W_I16 i = 0; i < MenuNum.size(); i++)
        {
            if (0x00 == MenuNum[i])
                iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x01", 6));    // 读码
            else if (0x01 == MenuNum[i])
                iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x02", 6));    // 清码
            else if (0x02 == MenuNum[i])
                iMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x03", 6));    // 读数据流
            else if (0x03 == MenuNum[i])
                iMenu.AddOneItem(CBinary("\x53\x20\x00\x01\x00\x02", 6));    // ECU规格信息
            else if (0x0f == MenuNum[i])
                iMenu.AddOneItem(STD_INFO_SPECIAL_FUNCTIONS);                // 特殊功能
        }
        if (Auto_Scan != m_AutoScan)
            iSelect = iMenu.ShowCtrl();
        else if (0xff == m_quick_Erase)
            return QuickClearTroubleCode();
        else
            return ReadTroubleCode();
        if (-1 == iSelect)
        {
            CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
#ifdef    __NT5600DPF__
            if (FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_EXITSYSTEM, DF_MB_YESNO) == DF_IDYES)
#endif
                break;
            continue;
        }

        switch (MenuNum[iSelect])
        {
        case 0x00:    // 读码
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
            rect = ReadTroubleCode();
            if (CErrorCode::EC_ECU_REFUSE == rect)
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_THE_SELECTED_FUNCTION_IS_NOT_SUPPORTED, DF_MB_OK, DT_LEFT);
            else if (0 >= SubaruBRZ_SET->DtcSet.size())
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_NO_FAULT_CODES_DETECTED, DF_MB_OK, DT_LEFT);
            else
                SubaruCanDtcShow();
            break;
        case 0x01:    // 清码
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
            rect = ClearTroubleCode();
            break;
        case 0x02:    // 数据流
            rect = DataStream();
            break;
        case 0x03:    // ECU信息
            rect = ReadEcuInfo();
            break;
        case 0x0f:    // 特殊功能
            rect = SpecialFunction();
            break;
        }
    }

    return rect;
}

W_ErrorCode CSubaruBaseCanApp::Read_computer_info()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CEcuInfoCtrl uiEcuInfo;
    CBinary binAns;
    string    strTemp, strItem, ROMID;
    CBinary bin22f182("\x22\xf1\x82", 3);
    CBinary binTitle("\x53\x20\x00\x01\x00\x02", 6);  // ECU规格信息

    W_I16    counts = 0;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    uiEcuInfo.InitCtrl(binTitle);

    binAns = SubaruCanSendReceive(bin22f182);
    if (binAns.GetByteCount() < 3 || binAns[0] != 0x62 || binAns[1] != 0xF1 || binAns[2] != 0x82)
    {
        FxShowMessageBox(binTitle, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
    }

    for (int i = 3; i < binAns.GetByteCount(); i++)
    {
        string m_str;
        char buf[100];
        sprintf(buf, "%02X", binAns[i]);
        m_str += buf;
        ROMID += m_str;
    }

    uiEcuInfo.AddOneItem(CBinary("\x53\x60\x01\x00\x0B\xDD", 6), ROMID);  // ROMID
    uiEcuInfo.AddOneItem(FxGetStdString(CBinary("\x53\x5E\x00\x00\x02\x24", 6)) + FxGetStdString(CBinary("\x53\x60\x01\x00\x07\xEB", 6)), "000000");  // 工具ID
    uiEcuInfo.AddOneItem(CBinary("\x01\x00\x00\x00\x03\x40", 6), FxGetStdString(CBinary("\x53\x60\x01\x00\x07\x95", 6)));  // 协议
    uiEcuInfo.ShowCtrl();

    return rect;
}



W_ErrorCode CSubaruBaseCanApp::ReadEcuInfo()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CEcuInfoCtrl uiEcuInfo;
    CBinary binAns;
    string    strTemp, strItem;
    CBinary bin22f18E("\x22\xf1\x8e", 3);
    CBinary bin22f189("\x22\xf1\x89", 3);
    CBinary binTitle("\x53\x20\x00\x01\x00\x02", 6);
    W_I16    counts = 0;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    uiEcuInfo.InitCtrl(binTitle);
    for (W_I16 i = 0; i < ECUInfo.size(); i++)
    {
        strTemp = "";
        W_I16 needLen = 0;
        binAns.Clear();
        if ("00170720" == ECUInfo[i][6])
        {
            strItem = FxGetStdString(CBinary("\x53\x20\x00\x01\x00\x03", 6));
            binAns = SubaruCanSendReceive(bin22f18E);
            needLen = 10;
        }
        else if ("01170720" == ECUInfo[i][6])
        {
            strItem = FxGetStdString(CBinary("\x53\x20\x00\x01\x00\x04", 6));
            binAns = SubaruCanSendReceive(bin22f189);
            needLen = 16;
        }
        //else if ("" == ECUInfo[i][6])//以下几个需要解决的模式
        //{  00270720 \01270720 \03360720 \03370720 \06270720 \07170720 \10851620    
        //}
        if (binAns.GetByteCount() > 0 && 0x7f != binAns[0])
        {
            char  Temp[10];
            strTemp = "";
            for (W_I16 j = 0; j < needLen && j + 3 < binAns.GetByteCount(); j++)
            {
                sprintf(Temp, "%c", binAns[3 + j]);
                strTemp += Temp;    //strTemp += binAns[3 + j];
            }
            uiEcuInfo.AddOneItem(strItem, strTemp);
            counts++;
        }
    }
    if (counts > 0)
        uiEcuInfo.ShowCtrl();
    else
        FxShowMessageBox(binTitle, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);

    return rect;
}

W_ErrorCode CSubaruBaseCanApp::New_ReadTroubleCode()
{
    W_ErrorCode            rect = CErrorCode::EC_SUCCESS;
    W_I16                IntTemp;
    CSearchString        SubaRead;
    vector<string>        vecStr;
    vector<W_I16>        ivecTemp;
    vector<CBinary>        cmdSet;
    map<W_I16, string>    StatusCmp;
    vector<vector<W_I16>> iReadListSet;
    vector<vector<string>> SearchReasault;
    CBinary m_bin;


    CFileManager profile;
    string            strMenu, strData, strTemp;

    strData = GetStringValue(Country_Ecu_File, Ecu_Id_Select[0]);
    if (!strData.size())
        return rect = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // 截取赋值   SetItemValue
    strMenu = profile.GetItemValue(Subaru_DTC_TTL, Subaru_ReadDtc_TXT);    // 获取要发送的读码命令

    SeparatorString(strMenu, ",", str_ReadDtc);

    for (int i = 0; i < str_ReadDtc.size(); i++)
    {
        m_bin = HexString2Binary(str_ReadDtc[i]);
        bin_ReadDtc.Append(m_bin);
    }

    CBinary binAns;
    CBinaryGroup bgAns;
    CBinary binTemp;
    vecStr.clear();
    SubaruBRZ_SET->DtcSet.clear();
    for (W_I16 i = 0; i < bin_ReadDtc.GetByteCount(); i++)
    {
        binAns = SubaruCanSendReceive(bin_ReadDtc[i]);     // 注意：ABS刹车系统回复的冻结帧数据过长，内存越界
        if (binAns.GetByteCount() > 0 && i == 0)  // 只插入19 02 AF
        {
            bgAns.Append(binAns);  // 插入 19 02 AF 的回复数据
        }
    }

    SubaruBRZ_SET->ReadFlag = true;
    if (Auto_Scan == m_AutoScan)
        CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    if (bgAns.GetByteCount() <= 0)
        return CErrorCode::EC_SUCCESS;
    for (W_I16 k = 0; k < bgAns.GetByteCount(); k++)
    {
        binAns = bgAns[k];
        if (binAns.GetByteCount() < 4)  // 控制位判断故障码
            return CErrorCode::EC_SUCCESS;
        else if (0x7f == binAns[0])
            return CErrorCode::EC_SUCCESS;
    }

    str_value_select.clear();  // 清空，避免返回后，故障码重叠
    for (W_I16 k = 0; k < bgAns.GetByteCount(); k++)
    {
        binAns = bgAns[k];
        binAns.DeleteByte(0);
        binAns.DeleteByte(0);
        binAns.DeleteByte(0);
        binAns.DeleteByte(0);  // 去除4个控制位后

        CBinary text;  // 临时状态文本
        CBinaryGroup bin_text_temp;  // 临时存放状态文本  (用于排序)
        vector<string>str_value_temp;  // 临时存放故障码回复数据  (用于排序)

        for (W_I16 i = 0; i + 2 < binAns.GetByteCount(); i += 4)    // 4个为1组，前2位为故障码，第3位为状态控制，第4位为填充位
        {
            int bin1 = binAns[i];
            int bin2 = binAns[i + 1];
            int bin3 = binAns[i + 2];
            int bin4 = binAns[i + 3];

            if (binAns[i] != 0x00 || binAns[i + 1] != 0x00)
            {
                if (binAns[i + 2] & 0x02)  //相与 为真这显示出对应的 02控制当前故障码 08控制历史故障码
                {
                    text = CBinary("\x53\x4D\x50\x27\x16\x20", 6);  // 当前
                    Dtcstatus_text.Append(text);   // 插入至状态文本容器中

                    // 前两位故障码数值拼接
                    char buf[500];
                    strTemp = "";
                    sprintf(buf, "%02X", binAns[i]);
                    strTemp += buf;
                    sprintf(buf, "%02X", binAns[i + 1]);
                    strTemp += buf;

                    str_value_select.push_back(strTemp);  // 存放故障码每个回复数据的头两位  (只把当前状态的数据插入这个容器中，用于排序)
                }
                else if (binAns[i + 2] & 0x08)
                {
                    text = CBinary("\x53\x5D\x00\x00\x38\xD0", 6);  // 历史
                    bin_text_temp.Append(text);

                    // 前两位故障码数值拼接
                    char buf[500];
                    strTemp = "";
                    sprintf(buf, "%02X", binAns[i]);
                    strTemp += buf;
                    sprintf(buf, "%02X", binAns[i + 1]);
                    strTemp += buf;

                    str_value_temp.push_back(strTemp);  // 存放故障码每个回复数据的头两位
                }
                else
                {
                    text = CBinary("\x53\x5D\x00\x00\x32\x4D", 6);  // 未定
                    bin_text_temp.Append(text);

                    // 前两位故障码数值拼接
                    char buf[500];
                    strTemp = "";
                    sprintf(buf, "%02X", binAns[i]);
                    strTemp += buf;
                    sprintf(buf, "%02X", binAns[i + 1]);
                    strTemp += buf;

                    str_value_temp.push_back(strTemp);  // 存放故障码每个回复数据的头两位
                }
            }
        }
        // 将临时的状态文本和回复数据插入需要的容器中
        for (int j = 0; j < bin_text_temp.GetByteCount(); j++)
        {
            Dtcstatus_text.Append(bin_text_temp[j]);
        }
        for (int n = 0; n < str_value_temp.size(); n++)
        {
            str_value_select.push_back(str_value_temp[n]);
        }
    }


    return rect;
}


W_ErrorCode CSubaruBaseCanApp::ReadTroubleCode()
{
    W_ErrorCode            rect = CErrorCode::EC_SUCCESS;
    W_I16                IntTemp;
    string                strTemp;
    CSearchString        SubaRead;
    vector<string>        vecStr;
    vector<W_I16>        ivecTemp;
    vector<CBinary>        cmdSet;
    map<W_I16, string>    StatusCmp;
    vector<vector<W_I16>> iReadListSet;
    vector<vector<string>> SearchReasault;

    if (FALSE == SubaRead.OpenTabFile(Subaru_Read_CBF))  //读码命令和方式
        return CErrorCode::EC_ECU_REFUSE;

    vecStr.push_back(SystemNum);
    SubaRead.SearchString(SearchReasault, FALSE, 0, 0, vecStr);   // 以 SystemNum 为内容筛选

    StatusCmp.clear();
    cmdSet.clear();
    for (W_I16 j = 0; j < SearchReasault.size(); j++)
    {
        for (W_I16 i = 0; i < ReadTroubleMode.size(); i++)
        {
            for (W_I16 k = 0; k < MenuShow.size(); k++)
            {
                if (MenuShow[k][6] == ReadTroubleMode[i])  // MenuShow[6] = SubaRead[i][1]
                {
                    strTemp = ReadTroubleMode[i].substr(0, 4);//新库07161520  06161520的值后8个字节是一样的 后面的状态会将前面的覆盖掉
                    IntTemp = StringToHex(strTemp);
                    StatusCmp[IntTemp] = FxGetStdString(CBinary("\x53\x4d", 2) + String2Binary(MenuShow[k][7]));   // 534d + MenuShow[7]
                }
            }
            if (ReadTroubleMode[i] == SearchReasault[j][1])  // 对 [1]~[6] 进行转换
            {
                ivecTemp.clear();
                strTemp = ReadTroubleMode[i].substr(0, 4);
                ivecTemp.push_back(StringToHex(strTemp));
                ivecTemp.push_back(StringToHex(SearchReasault[j][2]));
                ivecTemp.push_back(StringToHex(SearchReasault[j][3]));
                strTemp = SearchReasault[j][4];
                for (W_I16 k = 5; k < SearchReasault[j].size(); k++)
                {
                    ivecTemp.push_back(StringToHex(strTemp + SearchReasault[j][k]));
                    cmdSet.push_back(String2Binary(strTemp + SearchReasault[j][k]));
                }
                iReadListSet.push_back(ivecTemp);
            }
        }
    }
    if (cmdSet.size() <= 0)
        return CErrorCode::EC_SUCCESS;
    if ((0x01 == iReadListSet[0][2] || 0x02 == iReadListSet[0][2]) && ReadTroubleMode.size() > 1)  // 转换完成后进行判断
    {
        cmdSet.clear();
        cmdSet.push_back(String2Binary(SearchReasault[0][4] + SearchReasault[0][SearchReasault[0].size() - 1]));
        cmdSet.push_back(String2Binary(SearchReasault[1][4] + SearchReasault[1][SearchReasault[0].size() - 1]));
    }
    /*for (W_I16 i = 0; i < cmdSet.size(); i++)
    {
        for (W_I16 j = i + 1; j < cmdSet.size(); j++)
        {
            if (cmdSet[i] == cmdSet[j])
                cmdSet.erase(cmdSet.begin() + j--);
        }
    }*/

    CBinary binAns;
    CBinaryGroup bgAns;
    CBinary binTemp;
    vecStr.clear();
    SubaruBRZ_SET->DtcSet.clear();
    for (W_I16 i = 0; i < cmdSet.size(); i++)
    {
        binAns = SubaruCanSendReceive(cmdSet[i]);
        if (binAns.GetByteCount() > 0)
            bgAns.Append(binAns);
    }

    SubaruBRZ_SET->ReadFlag = true;
    if (Auto_Scan == m_AutoScan)
        CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
    if (bgAns.GetByteCount() <= 0)
        return CErrorCode::EC_SUCCESS;
    for (W_I16 k = 0; k < bgAns.GetByteCount(); k++)
    {
        binAns = bgAns[k];
        if (binAns.GetByteCount() < 4 && (0x01 == iReadListSet[0][1] || 0x02 == iReadListSet[0][1]))  // 通过控制位判断故障码
            return CErrorCode::EC_SUCCESS;
        else if ((0x01 == iReadListSet[0][1] || 0x02 == iReadListSet[0][1]) && 0x7f == binAns[0])
            return CErrorCode::EC_SUCCESS;
    }

    if (0x01 == iReadListSet[0][1] || 0x02 == iReadListSet[0][1] || 0x05 == iReadListSet[0][1])
    {
        for (W_I16 k = 0; k < bgAns.GetByteCount(); k++)
        {
            binAns = bgAns[k];
            binAns.DeleteByte(0);
            binAns.DeleteByte(0);
            binAns.DeleteByte(0);
            binAns.DeleteByte(0);  // 去除4个控制位后
            for (W_I16 i = 0; i + 2 < binAns.GetByteCount(); i += 4)
            {
                int bin1 = binAns[i];
                int bin2 = binAns[i + 1];
                int bin3 = binAns[i + 2];
                if ((binAns[i] != 0x00 || binAns[i + 1] != 0x00) && (binAns[i + 2] & iReadListSet[k][2]))//相与 为真这显示出对应的 02控制当前故障码 08控制过去故障码
                {
                    char buf[3];
                    strTemp = "";
                    sprintf(buf, "%02X", binAns[i]);
                    strTemp += buf;
                    sprintf(buf, "%02X", binAns[i + 1]);
                    strTemp += buf;

                    for (map<W_I16, string>::iterator pMap = StatusCmp.begin(); pMap != StatusCmp.end(); pMap++)
                    {
                        if (iReadListSet[k][0] == pMap->first)
                        {
                            vecStr.clear();
                            vecStr.push_back(strTemp);
                            vecStr.push_back(pMap->second);
                            SubaruBRZ_SET->DtcSet.push_back(vecStr);
                        }
                    }
                }
            }
        }

        // 排序 将当前故障显示到前面
        for (W_I16 i = 0; i < SubaruBRZ_SET->DtcSet.size() && StatusCmp.size() > 0; i++)
        {
            if (SubaruBRZ_SET->DtcSet[i][1] == FxGetStdString(CBinary("\x53\x4D\xDE\x3D\x16\x20", 6)))//当前故障:CBinary("\x53\x4D\x00\x00\x3A\xDC", 6)
            {
                W_I16 j = 0;
                vector<string> vecTemp;
                for (j = 0; j < SubaruBRZ_SET->DtcSet.size() && SubaruBRZ_SET->DtcSet[j][1] == FxGetStdString(CBinary("\x53\x4D\xDE\x3D\x16\x20", 6)); j++);
                if (j >= SubaruBRZ_SET->DtcSet.size())
                    break;
                vecTemp = SubaruBRZ_SET->DtcSet[i];
                while (j <= i)
                {
                    vecStr = SubaruBRZ_SET->DtcSet[j];
                    SubaruBRZ_SET->DtcSet[j++] = vecTemp;
                    vecTemp = vecStr;
                }
            }
        }
    }
    else if (0x04 == iReadListSet[0][1])
    {
        for (W_I16 counts = 0; counts < bgAns.GetByteCount(); counts++)
        {
            binAns = bgAns[counts];
            if (binAns.GetByteCount() < 4 || 0x7f == binAns[0])
                continue;
            binAns.DeleteByte(0);
            binAns.DeleteByte(0);
            ivecTemp.clear();
            for (W_I16 i = 0; i < iReadListSet.size(); i++)
            {
                binTemp = cmdSet[counts];
                strTemp = BinaryToString_MO(binTemp);
                IntTemp = StringToHex(strTemp);
                if (IntTemp == iReadListSet[i][3])
                {
                    ivecTemp = iReadListSet[i];
                    break;
                }
            }
            for (W_I16 i = 0; i + 1 < binAns.GetByteCount() && ivecTemp.size() > 0; i += 2)
            {
                if (binAns[i] != 0x00 || binAns[i + 1] != 0x00)
                {
                    char buf[3];
                    strTemp = "";
                    sprintf(buf, "%02X", binAns[i]);
                    strTemp += buf;
                    sprintf(buf, "%02X", binAns[i + 1]);
                    strTemp += buf;
                    for (map<W_I16, string>::iterator pMap = StatusCmp.begin(); pMap != StatusCmp.end(); pMap++)
                    {
                        if (ivecTemp[0] == pMap->first)
                        {
                            vecStr.clear();
                            vecStr.push_back(strTemp);
                            vecStr.push_back(pMap->second);
                            SubaruBRZ_SET->DtcSet.push_back(vecStr);
                        }
                    }
                }
            }
        }
    }

    return rect;
}


W_ErrorCode CSubaruBaseCanApp::New_SubaruCanDtcShow()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CBinary            binTemp;
    string            strTemp, strData, strMenu;
    CFileManager profile;
    CSearchString    SubaDTC;
    vector<string>    vecStr;
    vector<vector<string>> SearchResault;


    for (int i = 0; i < str_value_select.size(); i++)  // 筛选搜索项
    {
        CBinary m_id = CBinary("\x00\x00", 2);
        CBinary Dtc_id = HexString2Binary(System_Search_Id);  // 选菜单时记住的点击项索引
        CBinary value_id = HexString2Binary(str_value_select[i]);  // 读码命令回复的数据

        Dtc_id += m_id;
        Dtc_id += value_id;
        Dtc_id += CBinary("\x00\x07", 2);   // Suba_Dtc_Dex_ 中拼接的三项

        Dtcvalue_select.Append(Dtc_id);
    }


    for (int i = 0; i < Dtcvalue_select.GetByteCount(); i++)
    {
        binTemp = CBinary(STD_STANDARD"\x13", 6);    //请参考车辆维修手册

        strData = GetStringValue(Suba_DTC_NEW_CBF, Dtcvalue_select[i]);  // 搜索拼接的值
        if (!strData.size())
        {
            Dtctext_all.Append(binTemp);  // 如果搜索不到，则显示 "请参考车辆维修手册"
        }
        else
        {
            profile.InitManager(strData);  // 截取赋值   SetItemValue
            string strtemp = profile.GetItemValue(Subaru_DTC_TTL, Subaru_DtcText_TXT);  // DtcText 故障码显示文本

            CBinary bin_temp = HexString2Binary(strtemp);
            CBinary bin = CBinary("\x53\x5D", 2);
            bin += bin_temp;
            Dtctext_all.Append(bin);  // 插入筛选的 DtcText ,CBinary
        }
    }


    CTroubleCodeCtrl uiTrouble;
    uiTrouble.InitCtrl(Ttl_DtcNo);
    uiTrouble.m_bReportMode = false;

    // PCBU
    for (W_I16 i = 0; i < str_value_select.size(); i++)
    {

        char buf_f[100];
        char buf_s[100];
        char buf_t[100];
        char buf_fo[100];
        char* m_byte = &str_value_select[i][0];   // 取回复数据组中的每一项  char* 取址，取回复数据中每一项的string值

        // 分离为 char 单个字节
        char first_byte = m_byte[0];
        char second_byte = m_byte[1];
        char third_byte = m_byte[2];
        char fourth_byte = m_byte[3];

        // char -> string
        string str_first, str_second, str_third, str_fourth, str_one;
        sprintf(buf_f, "%c", first_byte);
        sprintf(buf_s, "%c", second_byte);
        sprintf(buf_t, "%c", third_byte);
        sprintf(buf_fo, "%c", fourth_byte);
        str_first += buf_f;   // string: 第1位
        str_second += buf_s; // string: 第2位
        str_third += buf_t; // string: 第3位
        str_fourth += buf_fo; // string: 第4位
        str_one = str_first + str_second;    // 回复数据组中的第一个字节拼接，用于判断 PCBU

        CBinary bin_one = HexString2Binary(str_one);
        //CBinary ding = CBinary("\x40",1);

        if (bin_one < CBinary("\x40", 1))   // 当第一个字节 < 0x40时，显示为 "P" += 回复命令
        {
            first_byte = 'P';
            string str;
            sprintf(buf_f, "%c", first_byte);  // char -> string
            str += buf_f;
            str += str_value_select[i];  // 拼接
            str_change_value.push_back(str);
        }
        else if (bin_one >= CBinary("\x40", 1) && bin_one < CBinary("\x80", 1))  // 当第一个字节 >= 0x40 && < 0x80时，显示为  ("C"+添加字节) += 回复命令
        {
            first_byte = 'C';
            string str, str_add;
            sprintf(buf_f, "%c", first_byte);
            str += buf_f;


            if (bin_one >= CBinary("\x40", 1) && bin_one < CBinary("\x50", 1))
            {
                str_add = "0";
            }
            else if (bin_one >= CBinary("\x50", 1) && bin_one < CBinary("\x60", 1))
            {
                str_add = "1";
            }
            else if (bin_one >= CBinary("\x60", 1) && bin_one < CBinary("\x70", 1))
            {
                str_add = "2";
            }
            else if (bin_one >= CBinary("\x70", 1) && bin_one < CBinary("\x80", 1))
            {
                str_add = "3";
            }
            str += str_add;  //  C 拼接 添加字节

            vector<string>vstr_temp;
            vstr_temp.push_back(str_second);
            vstr_temp.push_back(str_third);
            vstr_temp.push_back(str_fourth);

            // 拼接
            str += vstr_temp[0];  // 取除了头字节之外，剩下的三位字节   
            str += vstr_temp[1];
            str += vstr_temp[2];

            str_change_value.push_back(str);
        }
        else if (bin_one >= CBinary("\x80", 1) && bin_one < CBinary("\xC0", 1))  // 当第一个字节 >= 0x80 && < 0xC0时，显示为  ("B"+添加字节) += 回复命令
        {
            first_byte = 'B';
            string str, str_add;
            sprintf(buf_f, "%c", first_byte);
            str += buf_f;

            if (bin_one >= CBinary("\x80", 1) && bin_one < CBinary("\x90", 1))
            {
                str_add = "0";
            }
            else if (bin_one >= CBinary("\x90", 1) && bin_one < CBinary("\xA0", 1))
            {
                str_add = "1";
            }
            else if (bin_one >= CBinary("\xA0", 1) && bin_one < CBinary("\xB0", 1))
            {
                str_add = "2";
            }
            else if (bin_one >= CBinary("\xB0", 1) && bin_one < CBinary("\xC0", 1))
            {
                str_add = "3";
            }
            str += str_add;

            vector<string>vstr_temp;
            vstr_temp.push_back(str_second);
            vstr_temp.push_back(str_third);
            vstr_temp.push_back(str_fourth);

            str += vstr_temp[0];
            str += vstr_temp[1];
            str += vstr_temp[2];

            str_change_value.push_back(str);
        }
        else if (bin_one >= CBinary("\xC0", 1) && bin_one <= CBinary("\xFF", 1))  // 当第一个字节 >= 0xC0 && <= 0xFF时，显示为  ("U"+添加字节) += 回复命令
        {
            first_byte = 'U';
            string str, str_add;
            sprintf(buf_f, "%c", first_byte);
            str += buf_f;

            if (bin_one >= CBinary("\xC0", 1) && bin_one < CBinary("\xD0", 1))
            {
                str_add = "0";
            }
            else if (bin_one >= CBinary("\xD0", 1) && bin_one < CBinary("\xE0", 1))
            {
                str_add = "1";
            }
            else if (bin_one >= CBinary("\xE0", 1) && bin_one < CBinary("\xF0", 1))
            {
                str_add = "2";
            }
            else if (bin_one >= CBinary("\xF0", 1) && bin_one <= CBinary("\xFF", 1))
            {
                str_add = "3";
            }
            str += str_add;

            vector<string>vstr_temp;
            vstr_temp.push_back(str_second);
            vstr_temp.push_back(str_third);
            vstr_temp.push_back(str_fourth);

            str += vstr_temp[0];
            str += vstr_temp[1];
            str += vstr_temp[2];

            str_change_value.push_back(str);
        }
        uiTrouble.AddOneItem(str_change_value[i], FxGetStdString(Dtctext_all[i]), FxGetStdString(Dtcstatus_text[i]));
    }

    uiTrouble.EnableClearDTC(false);  // 清除故障码显示

    int select = uiTrouble.ShowCtrl();  // 显示故障码

    str_change_value.clear();
    Dtctext_all.Clear();
    Dtcstatus_text.Clear();


    //int select = uiTrouble.ShowCtrl();  // 显示故障码
    //while (1)
    //{
    //    int select = uiTrouble.ShowCtrl();
    //    if (select == -1)
    //    {
    //        return rect;
    //    }
    //    else
    //    {

    //    }
    //}


    return rect;
}


W_ErrorCode CSubaruBaseCanApp::SubaruCanDtcShow()
{
    W_ErrorCode        rect = CErrorCode::EC_SUCCESS;
    CBinary            binTemp;
    string            strTemp;
    CSearchString    SubaDTC;
    vector<string>    vecStr;
    vector<vector<string>> SearchResault;

    if (FALSE == SubaDTC.OpenTabFile(Subaru_DTC_CBF))  //故障码查询
        return CErrorCode::EC_DATA;

    vecStr.push_back(SystemNum);
    SubaDTC.SearchString(SearchResault, FALSE, 0, 0, vecStr);

    CTroubleCodeCtrl uiTrouble;
    uiTrouble.InitCtrl(Ttl_DtcNo);
#ifndef    __NT5600DEF__
    uiTrouble.m_bReportMode = false;
#endif
    for (W_I16 i = 0; i < SubaruBRZ_SET->DtcSet.size(); i++)
    {
        binTemp = CBinary(STD_STANDARD"\x13", 6);    //请参考车辆维修手册
        for (W_I16 j = 0; j < SearchResault.size(); j++)
        {
            if (CompareStringIsEquation(SubaruBRZ_SET->DtcSet[i][0], SearchResault[j][1]) && (SearchResault[j][2] != "0000"))//为ffff的时候才是需要的故障码内容
            {
                binTemp = String2Binary(SearchResault[j][3]);
                break;
            }
        }
        strTemp = SubaruBRZ_SET->DtcSet[i][0].at(0);
        char ch = StringToHex(strTemp) % 4 + 0x30;
        if (StringToHex(strTemp) / 4 < 1)
            strTemp = "P";
        else if (StringToHex(strTemp) / 4 < 2)
            strTemp = "C";
        else if (StringToHex(strTemp) / 4 < 3)
            strTemp = "B";
        else if (StringToHex(strTemp) / 4 < 4)
            strTemp = "U";
        strTemp += ch;
        for (W_I16 j = 1; j < SubaruBRZ_SET->DtcSet[i][0].length(); j++)
            strTemp += SubaruBRZ_SET->DtcSet[i][0].at(j);

        uiTrouble.AddOneItem(strTemp, FxGetStdString(binTemp), SubaruBRZ_SET->DtcSet[i][1]);
    }
    uiTrouble.ShowCtrl();

    return rect;
}

W_ErrorCode CSubaruBaseCanApp::New_ClearTroubleCode()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    vector<string> vecStr;
    vector<vector<string>> SearchResault;
    CSearchString SubaClear;
    CBinary binErase, binAns, binTemp;
    string strData, strMenu;
    CFileManager profile;


    if (DF_IDNO == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_TXT_MSG_ENGINEOFFKEYON, DF_MB_YESNO, DT_LEFT))  // 点火开关是否打开，引擎是否关闭
        return CErrorCode::EC_SUCCESS;
    if (DF_IDNO == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_INFO_DO_DELETE_DTC_FREEZE, DF_MB_YESNO, DT_CENTER))  // 故障码与冻结数据将被删除
        return CErrorCode::EC_SUCCESS;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);  // 与车辆通信中

    CBinary bin2202("\x22\x02\x05", 3);
    CBinary bin22f1("\x22\xF1\x00", 3);
    CBinary bin3101("\x31\x01\xFF\x00\x00\x00", 6);
    CBinary bin3103("\x31\x03\xFF\x00", 4);
    CBinary bin1003("\x10\x03", 2);
    CBinary bin1001("\x10\x01", 2);

    W_I16 iTemp = 0;

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin2202, binAns)))  // 31 01 FF
        return rect;
    if (binAns.GetByteCount() < 4 || binAns[0] != 0x62 || binAns[1] != 0x02 || binAns[2] != 0x05)
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))  // 31 01 FF
        return rect;
    if (binAns.GetByteCount() < 2 || binAns[0] != 0x50 || binAns[1] != 0x03)
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }


    binTemp.Clear();
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal())) //安全访问
    {
        SendAndRecive(bin22f1, binAns);
        SendAndRecive(bin1001, binAns);
        goto SENDDTC;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101, binAns)))  // 31 01 FF
        return rect;
    if (binAns.GetByteCount() < 4)
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() > 5 && 0x02 == binAns[5])
            break;
        else if (DF_IDCANCEL == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT))
            return CErrorCode::EC_SUCCESS;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1001, binAns)))
        return rect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    Sleep(500);
    FxShowMessageBox(STD_TTL_MSG_ERASEDTC, CBinary("\x53\x5F\x00\x00\x00\x14", 6), DF_MB_OK, DT_CENTER);  // 清码命令已执行
    Sleep(1000);


SENDDTC:
    // 读取故障码命令
    strData = GetStringValue(Country_Ecu_File, Ecu_Id_Select[0]);
    if (!strData.size())
        return rect = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // 截取赋值   SetItemValue
    strMenu = profile.GetItemValue(Subaru_DTC_TTL, Subaru_ReadDtc_TXT);    // 获取要发送的读码命令

    SeparatorString(strMenu, ",", str_ReadDtc);

    for (int i = 0; i < str_ReadDtc.size(); i++)
    {
        CBinary m_bin = HexString2Binary(str_ReadDtc[i]);
        bin_ReadDtc.Append(m_bin);
    }


    for (W_I16 i = 0; i < bin_ReadDtc.GetByteCount(); i++)
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin_ReadDtc[i], binAns)))
            return rect;
        if (i == 0)  // 19 02 AF
        {
            if (binAns.GetByteCount() < 3 || binAns.GetByteAt(0) != 0x59 || binAns.GetByteAt(1) != 0x02)
            {
                FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_ECU_REFUSE;
            }
        }
        else  // 19 05 FF
        {
            if (binAns.GetByteCount() < 3 || binAns.GetByteAt(0) != 0x59 || binAns.GetByteAt(1) != 0x05)
            {
                FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_ECU_REFUSE;
            }
        }
    }

    str_ReadDtc.clear();
    bin_ReadDtc.Clear();
    //FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_MSG_DONE, DF_MB_OK, DT_CENTER);
    //FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_MSG_TurnOnWithOff, DF_MB_OK, DT_LEFT);
    return rect;
}


W_ErrorCode CSubaruBaseCanApp::ClearTroubleCode()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    vector<string> vecStr;
    vector<vector<string>> SearchResault;
    CSearchString SubaClear;
    CBinary binErase, binAns, binTemp;

    ReadTroubleCode();
    if (SubaruBRZ_SET->DtcSet.size() <= 0)
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_MSG_NoclearDtc, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_SUCCESS;
    }

    if (FALSE == SubaClear.OpenTabFile(Subaru_Clear_CBF))  //清码命令查询
        return CErrorCode::EC_DATA;
    vecStr.push_back(SystemNum);
    SubaClear.SearchString(SearchResault, FALSE, 0, 0, vecStr);

    vecStr.clear();
    for (W_I16 i = 0; i < MenuShow.size(); i++)
    {
        if ("e9021620" == MenuShow[i][7])   // 与Menushow[7]比较
            vecStr.push_back(MenuShow[i][6]);
    }
    if (vecStr.size() <= 0)
        vecStr.push_back("02271220");
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        for (W_I16 j = 0; j < vecStr.size(); j++)
        {
            if (SearchResault[i][1] == vecStr[j])
            {
                binErase = String2Binary(SearchResault[i][2]);
                break;
            }
        }
    }
    if (DF_IDNO == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_TXT_MSG_ENGINEOFFKEYON, DF_MB_YESNO, DT_LEFT))
        return CErrorCode::EC_SUCCESS;
    if (DF_IDNO == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_INFO_DO_DELETE_DTC_FREEZE, DF_MB_YESNO, DT_CENTER))
        return CErrorCode::EC_SUCCESS;

    if (binErase.GetByteCount() < 0 && SearchResault.size() > 0)
        binErase = String2Binary(SearchResault[0][2]);

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (binErase.GetByteCount() > 0 && CBinary("\x31\x01\xFF\x00", 4) != binErase)
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binErase, binAns)))
            return rect;
    }
    else    //安全访问方式
    {
        CBinary bin3101("\x31\x01\xFF\x00\x00\x00", 6);
        CBinary bin3103("\x31\x03\xFF\x00", 4);
        CBinary bin1001("\x10\x01", 2);
        W_I16 iTemp = 0;

        binTemp.Clear();
        if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
            return rect;

        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101, binAns)))
            return rect;
        if (binAns.GetByteCount() < 4)
        {
            FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_ECU_REFUSE;
        }
        DWORD iTimer = 0;
        while (1)
        {
            if (iTimer < GetTickCount())
            {
                iTimer = GetTickCount() + 110;
                if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103, binAns)))
                    return rect;
            }
            if (binAns.GetByteCount() > 5 && 0x02 == binAns[5])
                break;
            else if (DF_IDCANCEL == FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT))
                return CErrorCode::EC_SUCCESS;
        }
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1001, binAns)))
            return rect;
    }

    FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_MSG_DONE, DF_MB_OK, DT_CENTER);
    FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_MSG_TurnOnWithOff, DF_MB_OK, DT_LEFT);

    return rect;
}

W_ErrorCode CSubaruBaseCanApp::QuickClearTroubleCode()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    vector<string> vecStr;
    vector<vector<string>> SearchResault;
    CSearchString SubaClear;
    CBinary binErase, binAns, binTemp;

    ReadTroubleCode();
    if (SubaruBRZ_SET->DtcSet.size() <= 0)
        return CErrorCode::EC_SUCCESS;

    if (FALSE == SubaClear.OpenTabFile(Subaru_Clear_CBF))
        return CErrorCode::EC_DATA;
    vecStr.push_back(SystemNum);
    SubaClear.SearchString(SearchResault, FALSE, 0, 0, vecStr);

    vecStr.clear();
    for (W_I16 i = 0; i < MenuShow.size(); i++)
    {
        if ("e9021620" == MenuShow[i][7])
            vecStr.push_back(MenuShow[i][6]);
    }
    if (vecStr.size() <= 0)
        vecStr.push_back("02271220");
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        for (W_I16 j = 0; j < vecStr.size(); j++)
        {
            if (SearchResault[i][1] == vecStr[j])
            {
                binErase = String2Binary(SearchResault[i][2]);
                break;
            }
        }
    }
    if (binErase.GetByteCount() > 0 && CBinary("\x31\x01\xFF\x00", 4) != binErase)
    {
        binAns = SubaruCanSendReceive(binErase);
    }
    else    //安全访问方式
    {
        CBinary bin3101("\x31\x01\xFF\x00\x00\x00", 6);
        CBinary bin3103("\x31\x03\xFF\x00", 4);
        CBinary bin1001("\x10\x01", 2);
        W_I16 iTemp = 0;

        binTemp.Clear();
        if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
            return rect;

        binAns = SubaruCanSendReceive(bin3101);
        CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);
        if (binAns.GetByteCount() < 4)
            return CErrorCode::EC_ECU_REFUSE;

        DWORD iTimer = 0;
        DWORD ExitTimer = GetTickCount() + 5000;
        while (1)
        {
            if (iTimer < GetTickCount())
            {
                iTimer = GetTickCount() + 110;
                binAns = SubaruCanSendReceive(bin3103);
            }
            if ((binAns.GetByteCount() > 5 && 0x02 == binAns[5]) || ExitTimer < GetTickCount())
                break;
        }
        binAns = SubaruCanSendReceive(bin1001);
    }
    ReadTroubleCode();
    CErrorCode::SetLastError(CErrorCode::EC_SUCCESS);

    return rect;
}


W_ErrorCode CSubaruBaseCanApp::New_SubaruCanGetDsList()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    string strTemp;
    vector<string> CmdSet;
    map<W_I32, CBinary> AnsSet;
    W_I16 Flag;
    CBinary binTemp, binAns;

    /* 命令采集 去重 */
    for (W_I16 i = 0; i < GetDS.size(); i++)
    {
        if (GetDS[i].size() < 10)
        {
            GetDS.erase(GetDS.begin() + i--);
        }
        else
        {
            strTemp = GetDS[i][1];
            strTemp += GetDS[i][2];    // 拼接 [1] 和 [2]
            CmdSet.push_back(strTemp);
        }
    }
    for (W_I16 i = 0; i < CmdSet.size(); i++)  // 去重
    {
        for (W_I16 j = i + 1; j < CmdSet.size(); j++)
        {
            if (CmdSet[i] == CmdSet[j])
                CmdSet.erase(CmdSet.begin() + j--);
        }
    }

    W_I32 IntTemp;
    /* 发送所有数据流命令 */
    for (W_I16 i = 0; i < CmdSet.size(); i++)
    {
        FxProgressBar(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, 100.0 / (float)CmdSet.size() * i);
        IntTemp = StringToHex(CmdSet[i]);
        binAns = SubaruCanSendReceive(String2Binary(CmdSet[i]));
        if (binAns.GetByteCount() > 0 && 0x7f != binAns[0])
        {
            // 只保存回复的有效数据
            for (W_I16 j = 0; j < String2Binary(CmdSet[i]).GetByteCount() && binAns.GetByteCount() > 0; j++)
                binAns.DeleteByte(0);
            AnsSet[IntTemp] = binAns;  // 放入AnsSet中
        }
    }

    W_I32 IntTemp1, IntTemp2, counts = 0;
    vector<vector<string> > vecvecTemp;
    /* 第二次筛选 根据回复了的数据流命令 */
    for (W_I16 i = 0; i < GetDS.size(); i++)
    {
        Flag = 0;
        IntTemp = StringToHex(GetDS[i][5]);
        IntTemp1 = StringToHex(GetDS[i][6]);
        strTemp = GetDS[i][1];
        strTemp += GetDS[i][2];
        IntTemp2 = StringToHex(strTemp);
        for (map<W_I32, CBinary>::iterator pMap = AnsSet.begin(); pMap != AnsSet.end(); pMap++)
        {
            //if (pMap->first == IntTemp2 && pMap->second.GetByteCount() >= IntTemp + IntTemp1)// 回复了的命令 如果有效数据长度不够该条数据流所需要的长度,则不显示该数据流 (与参考设备处理方式不同)
            if (pMap->first == IntTemp2)//显示所有数据流    将 拼接 去重 后的数据放入容器
            {
                Flag = 1;
                break;
            }
        }
        if (Flag)
        {
            vecvecTemp.push_back(GetDS[i]);
        }
    }
    GetDS.clear();
    GetDS = vecvecTemp;
    vecvecTemp.clear();
    if (GetDS.size() <= 0)
        return CErrorCode::EC_DATA;

    return rect;

}


/* 筛选数据流 只进行一次 */
W_ErrorCode CSubaruBaseCanApp::SubaruCanGetDsList()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    string strTemp;
    vector<string> CmdSet;
    map<W_I32, CBinary> AnsSet;
    W_I16 Flag;
    CBinary binTemp, binAns;

    /* 命令采集 去重 */
    for (W_I16 i = 0; i < GetDS.size(); i++)
    {
        if (GetDS[i].size() < 10)
        {
            GetDS.erase(GetDS.begin() + i--);
        }
        else
        {
            strTemp = GetDS[i][1];
            strTemp += GetDS[i][2];    // 拼接 [1] 和 [2]
            CmdSet.push_back(strTemp);
        }
    }
    for (W_I16 i = 0; i < CmdSet.size(); i++)  // 去重
    {
        for (W_I16 j = i + 1; j < CmdSet.size(); j++)
        {
            if (CmdSet[i] == CmdSet[j])
                CmdSet.erase(CmdSet.begin() + j--);
        }
    }

    W_I32 IntTemp;
    /* 发送所有数据流命令 */
    for (W_I16 i = 0; i < CmdSet.size(); i++)
    {
        FxProgressBar(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, 100.0 / (float)CmdSet.size() * i);
        IntTemp = StringToHex(CmdSet[i]);
        binAns = SubaruCanSendReceive(String2Binary(CmdSet[i]));
        if (binAns.GetByteCount() > 0 && 0x7f != binAns[0])
        {
            // 只保存回复的有效数据
            for (W_I16 j = 0; j < String2Binary(CmdSet[i]).GetByteCount() && binAns.GetByteCount() > 0; j++)
                binAns.DeleteByte(0);
            AnsSet[IntTemp] = binAns;  // 放入AnsSet中
        }
    }

    W_I32 IntTemp1, IntTemp2, counts = 0;
    vector<vector<string> > vecvecTemp;
    /* 第二次筛选 根据回复了的数据流命令 */
    for (W_I16 i = 0; i < GetDS.size(); i++)
    {
        Flag = 0;
        IntTemp = StringToHex(GetDS[i][5]);
        IntTemp1 = StringToHex(GetDS[i][6]);
        strTemp = GetDS[i][1];
        strTemp += GetDS[i][2];
        IntTemp2 = StringToHex(strTemp);
        for (map<W_I32, CBinary>::iterator pMap = AnsSet.begin(); pMap != AnsSet.end(); pMap++)
        {
            //if (pMap->first == IntTemp2 && pMap->second.GetByteCount() >= IntTemp + IntTemp1)// 回复了的命令 如果有效数据长度不够该条数据流所需要的长度,则不显示该数据流 (与参考设备处理方式不同)
            if (pMap->first == IntTemp2)//显示所有数据流    将 拼接 去重 后的数据放入容器
            {
                Flag = 1;
                break;
            }
        }
        if (Flag)
        {
            vecvecTemp.push_back(GetDS[i]);
        }
    }
    GetDS.clear();
    GetDS = vecvecTemp;
    vecvecTemp.clear();
    if (GetDS.size() <= 0)
        return CErrorCode::EC_DATA;

    return rect;
}




W_ErrorCode CSubaruBaseCanApp::All_DTS()
{
    W_ErrorCode     rect = CErrorCode::EC_SUCCESS;
    W_I16 iRet;
    CBinary binAns;


    CDataStreamCtrl uiDataStream;
    uiDataStream.SetReferenceCol(false);
    uiDataStream.InitCtrl(Subaru_TTL_ReadData);


    // 发送所有的命令
    for (int i = 0; i < Dts_All_info.size() - 836; i++)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
        binAns = SubaruCanSendReceive(CBinary("\x22", 1) + Dts_All_info[i][2]);
        //CBinary m_bin = Dts_All_info[i][2];
        //if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7F || binAns[0] != 0x62 || binAns[1] != m_bin[0] || binAns[2] != m_bin[1])
        //{
        //    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);  // 通讯错误
        //    return CErrorCode::EC_COMMUNICATION;
        //}

        uiDataStream.AddOneItem(FxGetStdString(CBinary("\x53\x5D", 2) + Dts_All_info[i][0]), FxGetStdString(CBinary("\x53\x5D", 2) + Dts_All_info[i][1]));
    }



    while (1)
    {
        iRet = uiDataStream.ShowCtrl();
        if (iRet == -1)
        {
            break;
        }
        for (int i = 0; i < Dts_All_info.size() - 836; i++)
        {
            binAns = SubaruCanSendReceive(CBinary("\x22", 1) + Dts_All_info[i][2]);
        }
    }
    return rect;
}



W_ErrorCode CSubaruBaseCanApp::Make_DTS()
{
    W_ErrorCode     rect = CErrorCode::EC_SUCCESS;
    W_I16 iSelect, iRet;
    string str_ret;
    CBinary binAns;

    CMultiSelectCtrl uiMut;
    uiMut.InitCtrl(FxGetStdString(Subaru_MSG_SelectDS));


    //map<string,int>temp;
    //map<int,string>m_temp;
    //for (int i = 0; i < Dts_Pid.size(); i++)
    //{
    //    temp.insert(pair<string,int>(str_Dts_Name[i],i));  // 插入名称和和名称对应的序号值
    //}

    //for (map<string,int>::iterator it = temp.begin(); it != temp.end(); it++)
    //{
    //    m_temp.insert(pair<int,string>(it->second,it->first));  // 以序号值排序
    //}

    //for (map<int ,string>::iterator it1 =m_temp.begin(); it1 != m_temp.end(); it1++)
    //{
    //    Dts_Pid_map.insert(pair<int ,CBinary>(it1->first,Dts_Pid[it1->first]));
    //    //Dts_Pid_map.Append(Dts_Pid[it1->first]); // 序号值对应的命令插入至容器
    //}  // 命令的顺序与名称的顺序相同且不重复

    //for (map<int,string>::iterator it = m_temp.begin(); it != m_temp.end(); it++)
    //{
    //    uiMut.AddOneItem(FxGetStdString(CBinary("\x53\x5D",2) + HexString2Binary(it->second)));   // 多项选择框添加名称项
    //}



    for (int i = 0; i < Dts_All_info.size(); i++)
    {
        CBinary binname = Dts_All_info[i][0];
        uiMut.AddOneItem(FxGetStdString(CBinary("\x53\x5D", 2) + binname));   // 多项选择框添加名称项
    }


    while (1)
    {
        iSelect = uiMut.ShowCtrl();  // 显示数据流名称
        if (-1 == iSelect)
        {
            Dts_All_info.clear();
            break;
        }

        //Dts_Pid_show.Clear();
        //Dts_info.clear();
        //for (W_I16 i = 0; i < Dts_Pid_map.GetByteCount(); i++)  // 命令的顺序与名称的顺序相同且不重复
        //{
        //    if (uiMut.GetSel(i) > 0)
        //    {
        //        Dts_Pid_show.Append(CBinary("\x22",1) + Dts_Pid_map[i]);  // 把选中的命令插入容器
        //        Dts_select_search.Append(Dts_search[i]);  // 把选中的搜索项插入容器
        //        Dts_info.insert(pair<string,string>(str_Dts_Name[i],str_Dts_Unit[i]));   // 将选中的数据的名称和单位插入map中
        //    }
        //}

        //map<int,CBinary>::iterator it = Dts_Pid_map.begin();

        //for (map<int,CBinary>::iterator it = Dts_Pid_map.begin(); it != Dts_Pid_map.end(); it++)
        //{
        //    for (int i = 0; i < Dts_Pid_map.size(); i++)
        //    {
        //        if (uiMut.GetSel(i) > 0)
        //        {
        //            Dts_Pid_show.Append(CBinary("\x22",1) + it->second);  // 把选中的命令插入容器
        //            Dts_select_search.Append(Dts_search[it->first]);  // 把选中的搜索项插入容器
        //            Dts_info.insert(pair<string,string>(str_Dts_Name[it->first],str_Dts_Unit[it->first]));   // 将选中的数据的名称和单位插入map中
        //            break;
        //        }
        //    }
        //}

        for (int i = 0; i < Dts_All_info.size(); i++)
        {
            if (uiMut.GetSel(i) > 0)  // 选中的项
            {
                CBinary binname, binunit, binpid, binsearch;
                CBinaryGroup bingroup;
                binname = Dts_All_info[i][0];
                binunit = Dts_All_info[i][1];
                binpid = Dts_All_info[i][2];
                binsearch = Dts_All_info[i][3];
                bingroup.Append(binname);
                bingroup.Append(binunit);
                bingroup.Append(binpid);
                bingroup.Append(binsearch);
                Dts_select_info.push_back(bingroup);  // 信息插入容器中

                //Dts_Pid_show.Append(CBinary("\x22",1) + it->second);  // 把选中的命令插入容器
                //Dts_select_search.Append(Dts_search[it->first]);  // 把选中的搜索项插入容器
                //Dts_info.insert(pair<string,string>(str_Dts_Name[it->first],str_Dts_Unit[it->first]));   // 将选中的数据的名称和单位插入map中
                //break;
            }
        }




        CDataStreamCtrl uiDataStream;
        uiDataStream.SetReferenceCol(false);
        uiDataStream.InitCtrl(Subaru_TTL_ReadData);

        //for(map<string,string>::iterator it = Dts_info.begin(); it != Dts_info.end(); it++)  // 添加项数据流名称和单位
        //{
        //    CBinary bin_name = HexString2Binary(it->first);
        //    CBinary bin_unit = HexString2Binary(it->second);
        //    uiDataStream.AddOneItem(FxGetStdString(CBinary("\x53\x5D",2) + bin_name),FxGetStdString(CBinary("\x53\x5D",2) + bin_unit));
        //}

        for (int i = 0; i < Dts_select_info.size(); i++)
        {
            uiDataStream.AddOneItem(FxGetStdString(CBinary("\x53\x5D", 2) + Dts_select_info[i][0]), FxGetStdString(CBinary("\x53\x5D", 2) + Dts_select_info[i][1]));
        }


        // 发送选中的命令
        for (int i = 0; i < Dts_select_info.size(); i++)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
            binAns = SubaruCanSendReceive(CBinary("\x22", 1) + Dts_select_info[i][2]);
            CBinary m_bin = Dts_select_info[i][2];
            if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7F || binAns[0] != 0x62 || binAns[1] != m_bin[0] || binAns[2] != m_bin[1])
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);  // 通讯错误
                return CErrorCode::EC_COMMUNICATION;
            }
            str_ret = Express_DTS(binAns, Dts_select_info[i][3]); // 数据流算法处理
            uiDataStream.SetItemValue(i, str_ret);   // 设置数据流值
        }



        while (1)
        {
            //for(int i = 0; i < Dts_select_info.size(); i++)   // 循环更新发命令
            //{
            //    binAns = SubaruCanSendReceive(CBinary("\x22",1) + Dts_select_info[i][2]);
            //    CBinary m_bin = Dts_select_info[i][2];
            //    if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7F || binAns[0] != 0x62 || binAns[1] != m_bin[0] || binAns[2] != m_bin[1])
            //    {
            //        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);  // 通讯错误
            //        return CErrorCode::EC_COMMUNICATION;
            //    }
            //    str_ret = Express_DTS(binAns,Dts_select_info[i][3]); // 数据流算法处理
            //    uiDataStream.SetItemValue(i,str_ret);   // 设置数据流值
            //}

            iRet = uiDataStream.ShowCtrl(); // 显示数据流
            if (iRet == -1)
            {
                //Dts_All_info.clear();
                Dts_select_info.clear();
                break;
            }

            for (int i = 0; i < Dts_select_info.size(); i++)   // 循环更新发命令
            {
                binAns = SubaruCanSendReceive(CBinary("\x22", 1) + Dts_select_info[i][2]);
                CBinary m_bin = Dts_select_info[i][2];
                if (binAns.GetByteCount() <= 0 || binAns[0] == 0x7F || binAns[0] != 0x62 || binAns[1] != m_bin[0] || binAns[2] != m_bin[1])
                {
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);  // 通讯错误
                    return CErrorCode::EC_COMMUNICATION;
                }
                str_ret = Express_DTS(binAns, Dts_select_info[i][3]); // 数据流算法处理
                uiDataStream.SetItemValue(i, str_ret);   // 设置数据流值
            }

        }
    }
    return rect;
}

string CSubaruBaseCanApp::Express_DTS(CBinary binAns, CBinary binsearch)
{
    W_ErrorCode     rect = CErrorCode::EC_SUCCESS;
    string str_answer = "";
    float m_answer = 0;
    string strData, strMenu;
    CFileManager SubaDTS;


    strData.clear();
    strMenu.clear();
    string strMenu_conv, strMenu_bytepos, strMenu_bitsize, strMenu_mul, strMenu_div, strMenu_off, strMenu_signmove, strMenu_min, strMenu_max, strMenu_defv;
    int BytePos, BitSize, Significant;
    float    LsbMul, LsbDiv, Offset, Min, Max;
    strData = GetStringValue(Suba_Suba_DataStream_CBF, binsearch);
    if (!strData.size())
    {
        FxShowMessageBox(Subaru_TTL_ReadData, CBinary("\x53\x00\x00\x01\x01\x90", 6), DF_MB_OK, DT_CENTER);  // 打开文件失败
        return str_answer;
    }

    // 取算法信息
    SubaDTS.InitManager(strData);  // 截取赋值   SetItemValue
    strMenu_conv = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_ConvMethod_TXT);
    strMenu_bytepos = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_BitPos_TXT);
    strMenu_bitsize = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_BitSize_TXT);
    strMenu_mul = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_LsbMul_TXT);
    strMenu_div = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_LsbDiv_TXT);
    strMenu_off = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_Offset_TXT);
    strMenu_signmove = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_Significant_TXT);
    strMenu_min = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_Min_TXT);
    strMenu_max = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_Max_TXT);
    strMenu_defv = SubaDTS.GetItemValue(Subaru_Express_TTL, Subaru_PDefaultValue_TXT);

    // 临时vector
    vector<string>str_temp;
    str_temp.push_back(strMenu_conv);
    str_temp.push_back(strMenu_bytepos);
    str_temp.push_back(strMenu_bitsize);
    str_temp.push_back(strMenu_mul);
    str_temp.push_back(strMenu_div);
    str_temp.push_back(strMenu_off);
    str_temp.push_back(strMenu_signmove);
    str_temp.push_back(strMenu_min);
    str_temp.push_back(strMenu_max);
    str_temp.push_back(strMenu_defv);
    Express_All_info.push_back(str_temp);  // 插入容器内


    for (int i = 0; i < Express_All_info.size(); i++)
    {
        // 算法信息转换为int可运算
        BytePos = atoi(Express_All_info[i][1].c_str());
        BitSize = atoi(Express_All_info[i][2].c_str());
        Significant = atoi(Express_All_info[i][6].c_str());

        LsbMul = atof(Express_All_info[i][3].c_str());
        LsbDiv = atof(Express_All_info[i][4].c_str());
        Offset = atof(Express_All_info[i][5].c_str());
        Min = atof(Express_All_info[i][7].c_str());
        Max = atof(Express_All_info[i][8].c_str());


        if (Express_All_info[i][0] == "2")  // 算法模式2
        {
            str_answer = ConvMethod_2(binAns, BitSize, Significant, LsbMul, LsbDiv, Offset);  // 算法模式2对应算法
            break;
        }

    }


    return str_answer;
}


string CSubaruBaseCanApp::ConvMethod_2(CBinary binAns, int BitSize, int Significant, float LsbMul, float LsbDiv, float Offset)
{
    string str_answer;
    float m_answer;
    int binshow = 0;
    int binnum = BitSize / 8;
    binnum = BitSize % 8 > 0 ? binnum + 1 : binnum;  // 控制位个数算法  (Bitsize/8)有余数+1，没有余数等于本身
    int AnsNum = 2 + binnum;

    // 取选中的数据的回复位
    for (int i = 3; i <= AnsNum; i++)
    {
        binshow = (binAns.GetByteAt(i) << (AnsNum - i) * 8) + binshow;   // 左移数据进行拼接
    }

    float temp_value = (binshow * LsbMul / LsbDiv) + (Offset);
    float pow_value = pow(10.0, Significant);  // 求对应的次幂
    m_answer = temp_value / pow_value;    // 数据对应算法

    char buf[100];
    sprintf(buf, "%.2f", m_answer);   // 保留2位小数
    str_answer += buf;

    return str_answer;
}




W_ErrorCode CSubaruBaseCanApp::New_DataStream()
{
    W_ErrorCode     rect = CErrorCode::EC_SUCCESS;
    CSearchString SubaMenuShow;
    vector<string> vctStr, vecCmd;
    vector<vector<string>> SearchResault;
    CSearchString SubaDTS_Dex;
    CFileManager SubaDTS;
    string strData, strMenu;

    vector<string>str_Dts_Pid;

    W_U8 iFlag = 0;
    W_I16 MenuCode = 0;


    if (FALSE == SubaDTS_Dex.OpenTabFile(Suba_DataStream_Dex_CBF))  //   数据流索引文件
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }

    vctStr.push_back(System_Search_Id);  //  插入系统索引
    SubaDTS_Dex.SearchString(SearchResault, FALSE, 1, 1, vctStr);  // 搜索内容
    if (SearchResault.size() <= 0)
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }

    // 拼接DTS索引项
    for (int j = 0; j < SearchResault.size(); j++)
    {
        for (int k = 0; k < SearchResault.size(); k++)
        {
            // 建立临时变量接收拼接值
            string dtstemp = SearchResault[j][k + 1] + SearchResault[j][k + 2] + SearchResault[j][k + 3];    // 拼接1,2,3项
            CBinary bintemp = HexString2Binary(dtstemp);
            Dts_search.Append(bintemp);
            break;
        }
    }


    // 搜索索引项
    for (int i = 0; i < Dts_search.GetByteCount(); i++)
    {
        strData.clear();
        strMenu.clear();
        string strMenu_u, strMenu_p;
        CBinary binname, binpid, binunit;
        CBinaryGroup bingroup;
        strData = GetStringValue(Suba_Suba_DataStream_CBF, Dts_search[i]);
        if (!strData.size())
            return rect = CErrorCode::EC_DATA;
        SubaDTS.InitManager(strData);  // 截取赋值   SetItemValue
        strMenu = SubaDTS.GetItemValue(Subaru_DS_TTL, Subaru_Name_TXT);    // 数据流名称
        strMenu_u = SubaDTS.GetItemValue(Subaru_DS_TTL, Subaru_Unit_TXT);    // 数据流单位
        strMenu_p = SubaDTS.GetItemValue(Subaru_CMD_TTL, Subaru_Pid_TXT);    // 数据流发送命令，不加\x22
        binname = HexString2Binary(strMenu);
        binpid = HexString2Binary(strMenu_p);
        binunit = HexString2Binary(strMenu_u);

        bingroup.Append(binname);
        bingroup.Append(binunit);
        bingroup.Append(binpid);
        bingroup.Append(Dts_search[i]);

        Dts_All_info.push_back(bingroup);  // 插入容器中

        //str_Dts_Name.push_back(strMenu);
        //str_Dts_Unit.push_back(strMenu_u);
        //str_Dts_Pid.push_back(strMenu_p);

        //CBinary bin ;
        //bin = HexString2Binary(str_Dts_Name[i]);   // 名称
        //Dts_Name.push_back(bin);

        //bin = HexString2Binary(str_Dts_Unit[i]);   // 单位
        //Dts_Unit.push_back(bin);

        //bin = HexString2Binary(str_Dts_Pid[i]);   // 发送命令
        //Dts_Pid.push_back(bin);
    }


    // 去重
    for (int i = 0; i < Dts_All_info.size(); i++)
    {
        for (int j = i + 1; j < Dts_All_info.size(); j++)
        {
            if (Dts_All_info[i][0] == Dts_All_info[j][0])
            {
                Dts_All_info.erase(Dts_All_info.begin() + j--);
            }
        }
    }


    CMenuCtrl menu;
    menu.InitCtrl(Subaru_TTL_ReadData);
    menu.AddOneItem(STD_TXT_MSG_ALL_DATA);
    menu.AddOneItem(STD_TTL_MNU_CUSTOM_LIST);
    while (1)
    {
        int select = menu.ShowCtrl();
        if (select == -1)
        {
            break;
        }
        else if (select == 0)
        {
            rect = All_DTS();  //  所有数据流
            break;
        }
        else
        {
            rect = Make_DTS();  // 定制数据流
            break;
        }
    }






    //strData.clear();
    //strMenu.clear();
    //binMainMenu.Clear();
    //strData = GetStringValue(Country_Vin_File,m_vehicle_type[iSelected]);
    //if(!strData.size())
    //    return ecRet = CErrorCode::EC_DATA;
    //profile.InitManager(strData);  // 截取赋值   SetItemValue
    //strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL,Subaru_VIN_ECUID);  

    //SeparatorString(strMenu,",",Vin_Ecu_Id);

    //SeparatorString(Vin_Ecu_Id[iSelMenuItem],"&",Vin_Ecu_Id_Select);   // 根据点击项对应的ECUID组



    // 无数据流
    if (GetDS.size() <= 0)   // SubaData库
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK);
        return rect;
    }

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ENTERSYS, DF_MB_NOBUTTON);
    // 第一次进入需要筛选
    if (DsSet.size() <= 0)  // 当前显示的数据流
    {
        rect = SubaruCanGetDsList();
        if (CErrorCode::EC_SUCCESS != rect)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK);
            return rect;
        }
        DsSet = GetDS;
    }

    CMenuCtrl uiMenu;
    while (1)
    {
        uiMenu.InitCtrl(Subaru_TTL_ReadData);
        uiMenu.AddOneItem(STD_TTL_MNU_CUSTOM_LIST);
        uiMenu.AddOneItem(STD_TXT_MSG_ALL_DATA);
        W_I16 iSelect = uiMenu.ShowCtrl();
        if (-1 == iSelect)
            return CErrorCode::EC_SUCCESS;

        if (1 == iSelect)
        {
            DsSet = GetDS;
        }
        else if (0 == iSelect)   //定制数据流
        {
            CMultiSelectCtrl uiMut;
            uiMut.InitCtrl(FxGetStdString(Subaru_MSG_SelectDS));
            for (W_I16 i = 0; i < GetDS.size(); i++)
                uiMut.AddOneItem(FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(GetDS[i][7])));  // 0x53,0x4d + SubaData[7] 数据流名称
            while (1)
            {
                iSelect = uiMut.ShowCtrl();
                if (-1 == iSelect)
                    return rect;

                DsSet.clear();
                for (W_I16 i = 0; i < GetDS.size(); i++)
                {
                    if (0 < uiMut.GetSel(i))
                        DsSet.push_back(GetDS[i]);   // 把选中的数据流项插入容器中
                }
                break;
            }
        }

        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
        CDataStreamCtrl uiDataStream;
        CBinaryGroup bgTemp;
        CBinaryGroup AllCmd;    //所有显示的数据流的命令
        uiDataStream.SetReferenceCol(false);
        uiDataStream.InitCtrl(Subaru_TTL_ReadData);
        for (W_I16 i = 0; i < DsSet.size(); i++)
        {
            bgTemp.Clear();
            for (W_I16 j = 7; j < 10; j++)
                bgTemp.Append(CBinary(Subaru_IDCAN, 2) + String2Binary(DsSet[i][j]));

            if (0x02 == bgTemp[1][2] && 0x07 == bgTemp[1][3] || ("00000000" != DsSet[i][12]))//if (0x00 == bgTemp[1][5] && 0x00 == bgTemp[1][4] || ("00000000" != DsSet[i][12]))
                uiDataStream.AddOneItem(FxGetStdString(bgTemp[0]));
            else
                uiDataStream.AddOneItem(FxGetStdString(bgTemp[0]), FxGetStdString(bgTemp[1]));
            AllCmd.Append(String2Binary(DsSet[i][1] + DsSet[i][2]));
        }

        W_I16 iRet = 0;
        CBinary binAns;
        vector<int> UpdataNum;
        map<W_I32, bool> Updataed;
        map<W_I32, bool> mapTemp;
        Updataed[0] = false;
        string strTemp;
        W_I16 iCounts = 0;
        DWORD iTimer = 0;
        bool is7F = false;
        while (1)
        {
            UpdataNum.clear();
            uiDataStream.GetItemUpdate(UpdataNum);

            if (iCounts >= UpdataNum.size())
                iCounts = 0;
            if (UpdataNum.size() > 0)
            {
                // 用于第一时间更新新出现的列
                mapTemp.clear();
                for (map<W_I32, bool>::iterator pMap = Updataed.begin(); pMap != Updataed.end(); pMap++)
                {
                    W_I16 i = 0;
                    for (i = 0; i < UpdataNum.size() && UpdataNum[i] != pMap->first; i++);
                    if (i < UpdataNum.size())
                        mapTemp[UpdataNum[i]] = pMap->second;
                }
                Updataed = mapTemp;
                for (W_I16 i = 0; i < UpdataNum.size(); i++)
                {
                    W_I16 flag = 0;
                    for (map<W_I32, bool>::iterator pMap = Updataed.begin(); pMap != Updataed.end(); pMap++)
                    {
                        if (pMap->first == UpdataNum[i])
                        {
                            flag = 1;
                            if (!pMap->second)
                            {
                                iCounts = i;
                                i = UpdataNum.size();
                            }
                            break;
                        }
                    }
                    if (0 == flag)
                    {
                        Updataed[UpdataNum[i]] = false;
                        iCounts = i;
                        break;
                    }
                }
                binAns = SubaruCanSendReceive(AllCmd[UpdataNum[iCounts]]);
                strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));//    "---"
                if (binAns.GetByteCount() >= 1 && 0x7f != binAns[0])
                {
                    is7F = false;
                    for (W_I16 j = 0; j < AllCmd[UpdataNum[iCounts]].GetByteCount() && binAns.GetByteCount() > 0; j++)
                        binAns.DeleteByte(0);
                    strTemp = SubaDataCanCal(binAns, DsSet[UpdataNum[iCounts]]);
                    if (strTemp.length() <= 0)
                        strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                }
                else
                {
                    is7F = true;
                }
                if ("-0" == strTemp)
                    strTemp = "0";
                uiDataStream.SetItemValue(UpdataNum[iCounts], strTemp);
                Updataed[UpdataNum[iCounts]] = true;
                iCounts++;

                // 检查一遍下面需要更新的是否有同样的命令的数据流,如果有直接使用这一次的回复计算结果
                for (W_I16 i = iCounts; i < UpdataNum.size(); i++)
                {
                    if (AllCmd[UpdataNum[i]] == AllCmd[UpdataNum[iCounts - 1]])
                    {
                        if (is7F)
                            strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                        else
                            strTemp = SubaDataCanCal(binAns, DsSet[UpdataNum[iCounts]]);
                        if (strTemp.length() <= 0)
                            strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                        if ("-0" == strTemp)
                            strTemp = "0";
                        uiDataStream.SetItemValue(UpdataNum[iCounts], strTemp);
                        Updataed[UpdataNum[iCounts]] = true;
                        iCounts++;
                    }
                }
            }
            iRet = uiDataStream.ShowCtrl();
            if (-1 == iRet)
                break;
        }
    }

    return rect;
}





W_ErrorCode CSubaruBaseCanApp::DataStream()
{
    W_ErrorCode    rect = CErrorCode::EC_SUCCESS;

    // 无数据流
    if (GetDS.size() <= 0)   // SubaData库
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK);
        return rect;
    }

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ENTERSYS, DF_MB_NOBUTTON);
    // 第一次进入需要筛选
    if (DsSet.size() <= 0)  // 当前显示的数据流
    {
        rect = SubaruCanGetDsList();
        if (CErrorCode::EC_SUCCESS != rect)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK);
            return rect;
        }
        DsSet = GetDS;
    }

    CMenuCtrl uiMenu;
    while (1)
    {
        uiMenu.InitCtrl(Subaru_TTL_ReadData);
        uiMenu.AddOneItem(STD_TTL_MNU_CUSTOM_LIST);
        uiMenu.AddOneItem(STD_TXT_MSG_ALL_DATA);
        W_I16 iSelect = uiMenu.ShowCtrl();
        if (-1 == iSelect)
            return CErrorCode::EC_SUCCESS;

        if (1 == iSelect)
        {
            DsSet = GetDS;
        }
        else if (0 == iSelect)   //定制数据流
        {
            CMultiSelectCtrl uiMut;
            uiMut.InitCtrl(FxGetStdString(Subaru_MSG_SelectDS));
            for (W_I16 i = 0; i < GetDS.size(); i++)
                uiMut.AddOneItem(FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(GetDS[i][7])));  // 0x53,0x4d + SubaData[7] 数据流名称
            while (1)
            {
                iSelect = uiMut.ShowCtrl();
                if (-1 == iSelect)
                    return rect;

                DsSet.clear();
                for (W_I16 i = 0; i < GetDS.size(); i++)
                {
                    if (0 < uiMut.GetSel(i))
                        DsSet.push_back(GetDS[i]);   // 把选中的数据流项插入容器中
                }
                break;
            }
        }

        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
        CDataStreamCtrl uiDataStream;
        CBinaryGroup bgTemp;
        CBinaryGroup AllCmd;    //所有显示的数据流的命令
        uiDataStream.SetReferenceCol(false);
        uiDataStream.InitCtrl(Subaru_TTL_ReadData);
        for (W_I16 i = 0; i < DsSet.size(); i++)
        {
            bgTemp.Clear();
            for (W_I16 j = 7; j < 10; j++)
                bgTemp.Append(CBinary(Subaru_IDCAN, 2) + String2Binary(DsSet[i][j]));

            if (0x02 == bgTemp[1][2] && 0x07 == bgTemp[1][3] || ("00000000" != DsSet[i][12]))//if (0x00 == bgTemp[1][5] && 0x00 == bgTemp[1][4] || ("00000000" != DsSet[i][12]))
                uiDataStream.AddOneItem(FxGetStdString(bgTemp[0]));
            else
                uiDataStream.AddOneItem(FxGetStdString(bgTemp[0]), FxGetStdString(bgTemp[1]));
            AllCmd.Append(String2Binary(DsSet[i][1] + DsSet[i][2]));
        }

        W_I16 iRet = 0;
        CBinary binAns;
        vector<int> UpdataNum;
        map<W_I32, bool> Updataed;
        map<W_I32, bool> mapTemp;
        Updataed[0] = false;
        string strTemp;
        W_I16 iCounts = 0;
        DWORD iTimer = 0;
        bool is7F = false;
        while (1)
        {
            UpdataNum.clear();
            uiDataStream.GetItemUpdate(UpdataNum);

            if (iCounts >= UpdataNum.size())
                iCounts = 0;
            if (UpdataNum.size() > 0)
            {
                // 用于第一时间更新新出现的列
                mapTemp.clear();
                for (map<W_I32, bool>::iterator pMap = Updataed.begin(); pMap != Updataed.end(); pMap++)
                {
                    W_I16 i = 0;
                    for (i = 0; i < UpdataNum.size() && UpdataNum[i] != pMap->first; i++);
                    if (i < UpdataNum.size())
                        mapTemp[UpdataNum[i]] = pMap->second;
                }
                Updataed = mapTemp;
                for (W_I16 i = 0; i < UpdataNum.size(); i++)
                {
                    W_I16 flag = 0;
                    for (map<W_I32, bool>::iterator pMap = Updataed.begin(); pMap != Updataed.end(); pMap++)
                    {
                        if (pMap->first == UpdataNum[i])
                        {
                            flag = 1;
                            if (!pMap->second)
                            {
                                iCounts = i;
                                i = UpdataNum.size();
                            }
                            break;
                        }
                    }
                    if (0 == flag)
                    {
                        Updataed[UpdataNum[i]] = false;
                        iCounts = i;
                        break;
                    }
                }
                binAns = SubaruCanSendReceive(AllCmd[UpdataNum[iCounts]]);
                strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));//    "---"
                if (binAns.GetByteCount() >= 1 && 0x7f != binAns[0])
                {
                    is7F = false;
                    for (W_I16 j = 0; j < AllCmd[UpdataNum[iCounts]].GetByteCount() && binAns.GetByteCount() > 0; j++)
                        binAns.DeleteByte(0);
                    strTemp = SubaDataCanCal(binAns, DsSet[UpdataNum[iCounts]]);
                    if (strTemp.length() <= 0)
                        strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                }
                else
                {
                    is7F = true;
                }
                if ("-0" == strTemp)
                    strTemp = "0";
                uiDataStream.SetItemValue(UpdataNum[iCounts], strTemp);
                Updataed[UpdataNum[iCounts]] = true;
                iCounts++;

                // 检查一遍下面需要更新的是否有同样的命令的数据流,如果有直接使用这一次的回复计算结果
                for (W_I16 i = iCounts; i < UpdataNum.size(); i++)
                {
                    if (AllCmd[UpdataNum[i]] == AllCmd[UpdataNum[iCounts - 1]])
                    {
                        if (is7F)
                            strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                        else
                            strTemp = SubaDataCanCal(binAns, DsSet[UpdataNum[iCounts]]);
                        if (strTemp.length() <= 0)
                            strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                        if ("-0" == strTemp)
                            strTemp = "0";
                        uiDataStream.SetItemValue(UpdataNum[iCounts], strTemp);
                        Updataed[UpdataNum[iCounts]] = true;
                        iCounts++;
                    }
                }
            }
            iRet = uiDataStream.ShowCtrl();
            if (-1 == iRet)
                break;
        }
    }

    return rect;
}


W_ErrorCode CSubaruBaseCanApp::Act_Test()
{
    W_ErrorCode     rect = CErrorCode::EC_SUCCESS;

    CSearchString Suba_ACT_Dex;
    vector<string> vctStr, vecCmd;
    vector<vector<string>> SearchResault;
    CFileManager SubaACT;
    string strData, strMenu;

    if (FALSE == Suba_ACT_Dex.OpenTabFile(Suba_ACT_Dex_CBF))  //   动作测试索引文件
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }

    vctStr.push_back(System_Search_Id);  //  插入系统索引
    Suba_ACT_Dex.SearchString(SearchResault, FALSE, 1, 1, vctStr);  // 搜索内容
    if (SearchResault.size() <= 0)
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }

    // 拼接DTS索引项
    for (int j = 0; j < SearchResault.size(); j++)
    {
        for (int k = 0; k < SearchResault.size(); k++)
        {
            // 建立临时变量接收拼接值
            string dtstemp = SearchResault[j][k + 1] + SearchResault[j][k + 2];    // 拼接1,2项
            CBinary bintemp = HexString2Binary(dtstemp);
            Act_search.Append(bintemp);
            break;
        }
    }


    map<string, int>m_temp;
    for (int i = 0; i < Act_search.GetByteCount(); i++)
    {
        strData.clear();
        strMenu.clear();
        strData = GetStringValue(Suba_ACT_CBF, Act_search[i]);
        if (!strData.size())
            return rect = CErrorCode::EC_DATA;
        SubaACT.InitManager(strData);  // 截取赋值   SetItemValue
        strMenu = SubaACT.GetItemValue(Subaru_Act_TTL, Subaru_ACTName_TXT);
        //CBinary bin = HexString2Binary(strMenu);
        //Act_Name.Append(bin);
        m_temp.insert(pair<string, int>(strMenu, i));
    }


    CMenuCtrl menu;
    for (map<string, int>::iterator it = m_temp.begin(); it != m_temp.end(); it++)
    {
        CBinary bin = HexString2Binary(it->first);
        menu.AddOneItem(FxGetStdString(CBinary("\x53\x5D", 2) + bin));
    }

    while (1)
    {
        int select = menu.ShowCtrl();
        if (select == -1)
        {
            break;
        }
        break;
    }





    return rect;
}


//数据流第十五列算法
static float CalList15(string CalID)
{
    float    x = 0;

    if (CalID == "0000000000000000") x = x;//case 0x0000
    //else if (CalID == "3fa295e9e1b089a0") x = x + 0.04;//case 0x0001
    else if (CalID == "3fe4faacd9e83e42");//NULL//case 0x0002
    else if (CalID == "3ff0000000000000");//NULL//case 0x0003
    else if (CalID == "3ff4cccccccccccd");//NULL//case 0x0004
    else if (CalID == "401b51eb851eb852");//NULL//case 0x0005
    else if (CalID == "4024000000000000") x = x + 10;//case 0x0006
    else if (CalID == "4025333333333333") x = x + 10.6;//case 0x0007
    else if (CalID == "402a99999999999a") x = x + 13.1;//case 0x0008
    else if (CalID == "4032000000000000");//NULL//case 0x0009
    else if (CalID == "4044d555318abc87");//NULL//case 0x000A
    else if (CalID == "4049000000000000");//NULL//case 0x000B
    else if (CalID == "4059000000000000");//NULL//case 0x000C
    else if (CalID == "4069000000000000");//NULL//case 0x000D
    else if (CalID == "bfe47ae147ae147b") x = x - 0.64;//case 0x000E
    else if (CalID == "bff0000000000000") x = x - 1;//case 0x000F
    else if (CalID == "bff47ae147ae147b") x = x - 1.28;//case 0x0010
    else if (CalID == "bffa99999999999a");//NULL//case 0x0011
    else if (CalID == "c000000000000000");//NULL//case 0x0012
    else if (CalID == "c008000000000000") x = x - 3;//case 0x0013
    else if (CalID == "c00999999999999a");//NULL//case 0x0014
    else if (CalID == "c00e0a3d70a3d70a");//NULL//case 0x0015
    else if (CalID == "c014000000000000") x = x - 5;//case 0x0016
    else if (CalID == "c017e80000000000");//NULL//case 0x0017
    else if (CalID == "c01999999999999a");//NULL//case 0x0018
    else if (CalID == "c01a000000000000");//NULL//case 0x0019
    else if (CalID == "c01b99999999999a");//NULL//case 0x001A
    else if (CalID == "c024000000000000") x = x - 10;//case 0x001B
    else if (CalID == "c02c000000000000");//NULL//case 0x001C
    else if (CalID == "c02e000000000000") x = x - 15;//case 0x001D
    else if (CalID == "c030000000000000") x = x - 16;//case 0x001E
    else if (CalID == "c031800000000000");//NULL//case 0x001F
    else if (CalID == "c0324ccccccccccd") x = x - 18.3;//case 0x0020
    else if (CalID == "c033f33333333333");//NULL//case 0x0021
    else if (CalID == "c034000000000000") x = x - 20;//case 0x0022
    else if (CalID == "c0374ccccccccccd");//NULL//case 0x0023
    else if (CalID == "c039000000000000");//NULL//case 0x0024
    else if (CalID == "c03c200000000000");//NULL//case 0x0025
    else if (CalID == "c03db33333333333");//NULL//case 0x0026
    else if (CalID == "c03e000000000000") x = x - 30;//case 0x0027
    else if (CalID == "c03f400000000000");//NULL//case 0x0028
    else if (CalID == "c040000000000000") x = x - 32;//case 0x0029
    else if (CalID == "c043f33333333333");//NULL//case 0x002A
    else if (CalID == "c044000000000000") x = x - 40;//case 0x002B
    else if (CalID == "c045000000000000") x = x - 42;//case 0x002C
    else if (CalID == "c049000000000000") x = x - 50;//case 0x002D
    else if (CalID == "c04b800000000000") x = x - 55;//case 0x002E
    else if (CalID == "c050000000000000") x = x - 64;//case 0x002F
    else if (CalID == "c059000000000000") x = x - 100;//case 0x0030
    else if (CalID == "c059451eb851eb85");//NULL//case 0x0031
    else if (CalID == "c059af851eb851ec");//NULL//case 0x0032
    else if (CalID == "c05e41eb851eb852");//NULL//case 0x0033
    else if (CalID == "c05f400000000000") x = x - 125;//case 0x0034
    else if (CalID == "c060000000000000") x = x - 128;//case 0x0035
    else if (CalID == "c0619f5c28f5c28f");//NULL//case 0x0036
    else if (CalID == "c062c00000000000") x = x - 150;//case 0x0037
    else if (CalID == "c069000000000000") x = x - 200;//case 0x0038
    else if (CalID == "c072c00000000000") x = x - 300;//case 0x0039
    else if (CalID == "c074000000000000") x = x - 320;//case 0x003A
    else if (CalID == "c079000000000000") x = x - 400;//case 0x003B
    else if (CalID == "c07999999999999a");//NULL//case 0x003C
    else if (CalID == "c08f400000000000") x = x - 1000;//case 0x003E
    else if (CalID == "c090000000000000") x = x - 1024;//case 0x003F
    else if (CalID == "c09f400000000000") x = x - 2000;//case 0x0040
    else if (CalID == "c0a999999999999a") x = x - 3276.8;//case 0x0041
    else if (CalID == "c0c3880000000000") x = x - 10000;//case 0x0042
    else if (CalID == "c0e0000000000000") x = x - 32768;//case 0x0043

    return x;
}


// 数据流第十四列算法
static float SubaDataCanCal_getYValue(vector<string>& VecStr, W_U32 x)
{
    float    y = 0;
    string    strCmpVal = VecStr[1];

    if (strCmpVal == "0000000000000000" || strCmpVal == "3ef0000000000000" || strCmpVal == "3f10000000000000" || strCmpVal == "3f14050140501405" || strCmpVal == "403f400000000000"
        || strCmpVal == "3f147ae147ae147b" || strCmpVal == "3f16058160581606" || strCmpVal == "3f48ffff06e4535a" || strCmpVal == "3f5999999999999a" || strCmpVal == "4034000000000000"
        || strCmpVal == "3f60000000000000" || strCmpVal == "3f66800000000000" || strCmpVal == "3f69000000000000" || strCmpVal == "3f70101010101010" || strCmpVal == "402e000000000000"
        || strCmpVal == "3f7547ae147ae148" || strCmpVal == "3f7800a7c5ac471b" || strCmpVal == "3f78fc504816f007" || strCmpVal == "3f8522a6f3f52fc2" || strCmpVal == "3ff160aa64c2f838"
        || strCmpVal == "3f8999999999999a" || strCmpVal == "3f8c30ab90df5837" || strCmpVal == "3f90000000000000" || strCmpVal == "3f90101010101010" || strCmpVal == "3ff10624dd2f1aa0"
        || strCmpVal == "3f901010187bf7c8" || strCmpVal == "3f90624dd2f1a9fc" || strCmpVal == "3f940cc78e9f6a94" || strCmpVal == "3f94141414141414" || strCmpVal == "3ff0a00000000000"
        || strCmpVal == "3f9414a4d2b2bfdb" || strCmpVal == "3f948f70b8667af6" || strCmpVal == "3f96161616161616" || strCmpVal == "3f96bb98c7e28241" || strCmpVal == "3fe93264c993264d"
        || strCmpVal == "3f96c16c16c16c17" || strCmpVal == "3fc2708ede54b48d" || strCmpVal == "3f989374bc6a7efa" || strCmpVal == "3f9a36e2eb1c432d" || strCmpVal == "3fe547ae147ae148"
        || strCmpVal == "3f9ec2ce4649906d" || strCmpVal == "3fa4000000000000" || strCmpVal == "3fa9000000000000" || strCmpVal == "3faac5c13fd0d068" || strCmpVal == "3fe4c80000000000"
        || strCmpVal == "3fb2122fad6cb535" || strCmpVal == "3fb41205bc01a36e" || strCmpVal == "3fbc800000000000" || strCmpVal == "3fbe000000000000" || strCmpVal == "3fe10624dd2f1aa0"
        || strCmpVal == "3fc16872b020c49c" || strCmpVal == "3fc1800000000000" || strCmpVal == "3fc2708ede54b48d" || strCmpVal == "3fc851eb851eb852" || strCmpVal == "3fdd180000000000"
        || strCmpVal == "3fc89374bc6a7efa" || strCmpVal == "3fcf3ffac1d29dc7" || strCmpVal == "3fcf400000000000" || strCmpVal == "3fd147ae147ae148" || strCmpVal == "3fdc28f5c28f5c29"
        || strCmpVal == "3fd1604189374bc7" || strCmpVal == "3fd4d4fdf3b645a2" || strCmpVal == "3fd4dd2f1a9fbe77" || strCmpVal == "3fd6666666666666" || strCmpVal == "3fd90624dd2f1aa0"
        || strCmpVal == "3fd6800000000000" || strCmpVal == "3fd8f5c28f5c28f6" || strCmpVal == "3ff8000000000000" || strCmpVal == "40039eb851eb851f" || strCmpVal == "4004000000000000"
        || strCmpVal == "400999999999999a" || strCmpVal == "4039000000000000" || strCmpVal == "4051c00000000000" || strCmpVal == "bfab51eb851eb852" || strCmpVal == "bfd4d555318abc87"
        || strCmpVal == "bfe1604189374bc7" || strCmpVal == "bff0000000000000" || strCmpVal == "3f3916872b020c4a")
        ;//ASCII码
    else if (strCmpVal == "3ee4f8b588e368f1") { y = x * 0.00001; }
    else if (strCmpVal == "3efffb480a5accd5") { y = x * 1.999 / 0xffff; }
    else if (strCmpVal == "3f00624dd2f1a9fc") { y = x * 2.048 / 0xffff; }
    else if (strCmpVal == "3f13ffff38b6a915") { y = x * 5.0 / 0xffff; }
    else if (strCmpVal == "3f1a36e2eb1c432d") { y = x * 0.0001; }
    else if (strCmpVal == "3f1ffb480a5accd5") { y = x * 0.000122; }
    else if (strCmpVal == "3f340000f084a6b2") { y = x * 20.0 / 0xffff; }
    else if (strCmpVal == "3f347ae147ae147b") { y = x * 0.08 / 255; }
    else if (strCmpVal == "3f3999999999999a") { y = x * 0.1 / 256; }
    else if (strCmpVal == "3f50000089705f41") { y = x * 64.0 / 0xffff; }
    else if (strCmpVal == "3f50624dd2f1a9fc") { y = x * 0.001; }
    else if (strCmpVal == "3f53ffffdda3e830") { y = x * 80.0 / 0xffff; }
    else if (strCmpVal == "3f54013fd741ac03") { y = x * 40.01 / 0x8000; if (x >= 0x8000) y = y - 80.02; }
    else if (strCmpVal == "3f59333344613f1b") { y = x * 50.4 / 0x8000; if (x >= 0x8000) y = y - 100.8; }
    else if (strCmpVal == "3f5c9870be72dbaf") { y = x * 0.001745329; }
    else if (strCmpVal == "3f5fff79c842fa51") { y = x * 0.001953; if (x & 0x1000) y = y - 15.998976; }
    else if (strCmpVal == "3f667ffffbb47d06") { y = x * 180.0 / 0xffff; }
    else if (strCmpVal == "3f6f3fffdb7e26b3") { y = x * 250.0 / 0xffff; }
    else if (strCmpVal == "3f70000000000000") { y = x * 256.0 / 65536; }
    else if (strCmpVal == "3f70624dd2f1a9fc") { y = x * 0.004; }
    else if (strCmpVal == "3f729888f861a60d") { y = x * 0.00454; }
    else if (strCmpVal == "3f72c0001353cd65") { y = x * 300.0 / 0xffff; }
    else if (strCmpVal == "3f7365dbe16e98b0") { y = x * 155.183 / 0x8000; if (x >= 0x8000) y = y - 310.366; }
    else if (strCmpVal == "3f740000225c17d0") { y = x * 320.0 / 65536; }
    else if (strCmpVal == "3f747ae147ae147b") { "0000000a" != VecStr[5] ? y = x * 0.005 : y = x * 327.5 / 0xffff; }
    else if (strCmpVal == "3f76f0068db8bac7") { y = x * 0.0056; if (x & 0x1000) y = y - 45.8752; }
    else if (strCmpVal == "3f79000019c511dc") { y = x * 400.0 / 0xffff; }
    else if (strCmpVal == "3f79d5640ee209e5") { y = x * 206.67 / 0x8000; if (x >= 0x8000) y = y - 413.34; }
    else if (strCmpVal == "3f7f400020365653") { y = x * 500.0 / 65536; }
    else if (strCmpVal == "3f80000000000000") { y = x * 1.992 / 255; }
    else if (strCmpVal == "3f802de00d1b7176") { y = x * 0.0079; }
    else if (strCmpVal == "3f84000000000000") { y = x * 640.0 / 0xffff; }
    else if (strCmpVal == "3f847ae147ae147b") { y = x * 0.01; }
    else if (strCmpVal == "3f8bf487fcb923a3") { x %= 0x0800; y = x * 0.01365; if (x >= 0x0400) y = y - 27.9552; }
    else if (strCmpVal == "3f93f7ced916872b") { y = x * 0.0195; }
    else if (strCmpVal == "3f94000000000000") { y = x * 5.0 / 256.0; }
    else if (strCmpVal == "3f941205bc01a36e") { y = x * 5.0 / 255.0; }
    else if (strCmpVal == "3f94141411b86ccc") { y = x * 5.0 / 255.0; }
    else if (strCmpVal == "3f942c3c9eecbfb1") { y = x * 0.0197; }
    else if (strCmpVal == "3f947ae147ae147b") { y = x * 0.02; }
    else if (strCmpVal == "3f96872b020c49ba") { y = x * 0.022; }
    else if (strCmpVal == "3f9999999999999a") { y = x * 0.025; }
    else if (strCmpVal == "3fa0000000000000") { y = x * 2048.0 / 0xffff; }
    else if (strCmpVal == "3fa41205bc01a36e") { y = x * 10.0 / 255; }
    else if (strCmpVal == "3fa47ae147ae147b") { y = x * 0.04; }
    else if (strCmpVal == "3fa6f0068db8bac7") { y = x * 11.4 / 255; }
    else if (strCmpVal == "3fa916872b020c4a") { y = x * 0.049; }
    else if (strCmpVal == "3fa999999999999a") { y = x * 0.05; }
    else if (strCmpVal == "3faccccccccccccd") { y = x * 0.05625; }
    else if (strCmpVal == "3fb0000000000000") { y = x * 16.0 / 256; }
    else if (strCmpVal == "3fb0624dd2f1a9fc") { y = x * 0.064; }
    else if (strCmpVal == "3fb29c779a6b50b1") { y = x * 18.57 / 255; }
    else if (strCmpVal == "3fb2bd3c36113405") { y = x * 0.0732; }
    else if (strCmpVal == "3fb4000000000000") { y = x * 19.92 / 255; }
    else if (strCmpVal == "3fb414141603efc6") { y = x * 20.0 / 255; }
    else if (strCmpVal == "3fb47ae147ae147b") { y = x * 20.4 / 255; }
    else if (strCmpVal == "3fb594af4f0d844d") { y = x * 21.5 / 255; }
    else if (strCmpVal == "3fb916872b020c4a") { y = x * 0.098; }
    else if (strCmpVal == "3fb999999999999a") { y = x * 0.1; }
    else if (strCmpVal == "3fb9db22d0e56042") { y = x * 0.101; }
    else if (strCmpVal == "3fbf7ced916872b0") { y = x * 0.123; }
    else if (strCmpVal == "3fc0000000000000") { y = x * 32.0 / 256.0; }
    else if (strCmpVal == "3fc0a3d70a3d70a4") { y = x * 0.13; }
    else if (strCmpVal == "3fc0b71d83489f56") { y = x * 0.1306; }
    else if (strCmpVal == "3fc0b71d83ea50b7") { y = x * 0.1306; }
    else if (strCmpVal == "3fc253a07b8b10c0") { y = x * 36.5 / 255; }
    else if (strCmpVal == "3fc8793dd97f62b7") { y = x * 0.1912; }
    else if (strCmpVal == "3fc89d495182a993") { y = x * 0.1923; }
    else if (strCmpVal == "3fc9000000000000") { y = x * 50.0 / 256.0; }
    else if (strCmpVal == "3fc999999999999a") { y = x * 51.0 / 255.0; }
    else if (strCmpVal == "3fd0000000000000") { y = x * 0.25; }
    else if (strCmpVal == "3fd0400000000000") { y = x * 0.2539; }
    else if (strCmpVal == "3fd0624dd2f1a9fc") { y = x * 65.28 / 255; }
    else if (strCmpVal == "3fd072b020c49ba6") { y = x * 0.257; }
    else if (strCmpVal == "3fd15cfaacd9e83e") { y = x * 69.5 / 256; }
    else if (strCmpVal == "3fd16872b020c49c") { y = x * 8912.9 / 0x8000; if (x >= 0x8000) y = y - 17825.8; }
    else if (strCmpVal == "3fd3333333333333") { y = x * 0.3; }
    else if (strCmpVal == "3fd3d70a3d70a3d7") { y = x * 0.31; }
    else if (strCmpVal == "3fd9000000000000") { y = x * 100.0 / 255.0; }
    else if (strCmpVal == "3fd916872b020c4a") { y = x * 100.0 / 255.0; }
    else if (strCmpVal == "3fd9191919191919") { y = x * 100.0 / 255.0; }
    else if (strCmpVal == "3fd91919195f2a3b") { y = x * 100.0 / 255.0; }
    else if (strCmpVal == "3fd91919ac79702e") { y = x * 100.0 / 255.0; }
    else if (strCmpVal == "3fd999999999999a") { y = x * 0.4; }
    else if (strCmpVal == "3fdf400000000000") { y = x * 125.0 / 255.0; }
    else if (strCmpVal == "3fe0000000000000") { y = x * 0.5; }
    else if (strCmpVal == "3fe1919191919192") { y = x * 140.0 / 255.0; }
    else if (strCmpVal == "3fe4000000000000") { y = x * 159.0 / 255.0; }
    else if (strCmpVal == "3fe9000000000000") { y = x * 0.78125; }
    else if (strCmpVal == "3fe999999999999a") { y = x * 0.8; }
    else if (strCmpVal == "3feccccccccccccd") { y = x * 0.9; }
    else if (strCmpVal == "3ff0000000000000") { y = x; }
    else if (strCmpVal == "3ff0624dd2f1a9fc") { y = x * 67108.0 / 65535.0; }
    else if (strCmpVal == "3ff0cccccccccccd") { y = x * 1.05; }
    else if (strCmpVal == "3ff160bf5d78811b") { y = x * 277.0 / 255.0; }
    else if (strCmpVal == "3ff47ae147ae147b") { y = x * 1.28; }
    else if (strCmpVal == "3ff4cccccccccccd") { y = x * 1.3; }
    else if (strCmpVal == "3ff4e978d4fdf3b6") { y = x * 1.307; }
    else if (strCmpVal == "3ff610624dd2f1aa") { y = x * 351.6 / 255; }
    else if (strCmpVal == "3ff7851eb851eb85") { y = x * 1.47; }
    else if (strCmpVal == "3ff999999999999a") { y = x * 1.6; }
    else if (strCmpVal == "3ffb851eb851eb85") { y = x * 1.72; }
    else if (strCmpVal == "4000000000000000") { y = x * 2.0; }
    else if (strCmpVal == "4008000000000000") { y = x * 3.0; }
    else if (strCmpVal == "4008c00000000000") { y = x * 3.09375; }
    else if (strCmpVal == "4014000000000000") { y = x * 5.0; }
    else if (strCmpVal == "4014008bcf64e5ec") { y = x * 327710.0 / 0xffff; }
    else if (strCmpVal == "4020000000000000") { y = x * 8.0; }
    else if (strCmpVal == "4024000000000000") { y = x * 10.0; }
    else if (strCmpVal == "4040000000000000") { y = x * 32.0; }
    else if (strCmpVal == "4049000000000000") { y = x * 50.0; }
    else if (strCmpVal == "4052c00000000000") { y = x * 75.0; }
    else if (strCmpVal == "4059000000000000") { y = x * 100.0; }
    else if (strCmpVal == "c059000000000000") { y = x * (-1.178); }

    y += CalList15(VecStr[2]);

    //if (strCmpVal == "0000000000000000") {;}//ASCII码//case 0x0000
    //else if (strCmpVal == "3ee4f8b588e368f1") {y = x * 0.00001;}//case 0x0001
    //else if (strCmpVal == "3ef0000000000000") {;}//NULL//case 0x0002
    //else if (strCmpVal == "3efffb480a5accd5") {y = x * 1.999 / 0xffff;}//case 0x0003
    //else if (strCmpVal == "3f00624dd2f1a9fc") {y = x * 2.048 / 0xffff;}//case 0x0004
    //else if (strCmpVal == "3f10000000000000") {;}//NULL//case 0x0005
    //else if (strCmpVal == "3f13ffff38b6a915") {y = x * 5.0 / 0xffff;}//case 0x0006
    //else if (strCmpVal == "3f14050140501405") {;}//NULL//case 0x0007
    //else if (strCmpVal == "3f147ae147ae147b") {;}//NULL//case 0x0008
    //else if (strCmpVal == "3f16058160581606") {;}//NULL//case 0x0009
    //else if (strCmpVal == "3f1a36e2eb1c432d") {y = x * 0.0001;}//case 0x000A
    //else if (strCmpVal == "3f1ffb480a5accd5") {y = x * 0.000122;}//case 0x000B
    //else if (strCmpVal == "3f340000f084a6b2") {y = x * 20.0 / 0xffff;}//case 0x000C
    //else if (strCmpVal == "3f347ae147ae147b") {y = x * 0.08 / 255;}//case 0x000D
    //else if (strCmpVal == "3f3916872b020c4a") {;}//NULL//case 0x000E
    //else if (strCmpVal == "3f3999999999999a") {y = x * 0.1 / 256;}//case 0x000F
    //else if (strCmpVal == "3f48ffff06e4535a") {;}//NULL//case 0x0010
    //else if (strCmpVal == "3f50000089705f41") {y = x * 64.0 / 0xffff;}//case 0x0011
    //else if (strCmpVal == "3f50624dd2f1a9fc") {y = x * 0.001;}//case 0x0012
    //else if (strCmpVal == "3f53ffffdda3e830") {y = x * 80.0 / 0xffff;}//case 0x0013
    //else if (strCmpVal == "3f54013fd741ac03") {y = x * 40.01 / 0x8000;if (x >= 0x8000) y = y - 80.02;}//case 0x0014
    //else if (strCmpVal == "3f59333344613f1b") {y = x * 50.4 / 0x8000;if (x >= 0x8000) y = y - 100.8;}//case 0x0015
    //else if (strCmpVal == "3f5999999999999a") {;}//NULL//case 0x0016
    //else if (strCmpVal == "3f5c9870be72dbaf") {y = x * 0.001745329;}//case 0x0017
    //else if (strCmpVal == "3f5fff79c842fa51") {y = x * 0.001953;if (x & 0x1000) y = y - 15.998976;}//case 0x0018
    //else if (strCmpVal == "3f60000000000000") {;}//NULL//case 0x0019
    //else if (strCmpVal == "3f667ffffbb47d06") {y = x * 180.0 / 0xffff;}//case 0x001A
    //else if (strCmpVal == "3f66800000000000") {;}//NULL//case 0x001B
    //else if (strCmpVal == "3f69000000000000") {;}//NULL//case 0x001C
    //else if (strCmpVal == "3f6f3fffdb7e26b3") {y = x * 250.0 / 0xffff;}//case 0x001D
    //else if (strCmpVal == "3f70000000000000") {y = x * 256.0 / 65536;}//case 0x001E
    //else if (strCmpVal == "3f70101010101010") {;}//NULL//case 0x001F
    //else if (strCmpVal == "3f70624dd2f1a9fc") {y = x * 0.004;}//case 0x0020
    //else if (strCmpVal == "3f729888f861a60d") {y = x * 0.00454;}//case 0x0021
    //else if (strCmpVal == "3f72c0001353cd65") {y = x * 300.0 / 0xffff;}//case 0x0022
    //else if (strCmpVal == "3f7365dbe16e98b0") {y = x * 155.183 / 0x8000;if (x >= 0x8000) y = y - 310.366;}//case 0x0023
    //else if (strCmpVal == "3f740000225c17d0") {y = x * 320.0 / 65536;}//case 0x0024
    //else if (strCmpVal == "3f747ae147ae147b") {"0000000a" != VecStr[5] ? y = x * 0.005 : y = x * 327.5 / 0xffff;}//case 0x0025
    //else if (strCmpVal == "3f7547ae147ae148") {;}//NULL//case 0x0026
    //else if (strCmpVal == "3f76f0068db8bac7") {y = x * 0.0056;if (x & 0x1000) y = y - 45.8752;}//case 0x0027
    //else if (strCmpVal == "3f7800a7c5ac471b") {;}//NULL//case 0x0028
    //else if (strCmpVal == "3f78fc504816f007") {;}//NULL//case 0x0029
    //else if (strCmpVal == "3f79000019c511dc") {y = x * 400.0 / 0xffff;}//case 0x002A
    //else if (strCmpVal == "3f79d5640ee209e5") {y = x * 206.67 / 0x8000;if (x >= 0x8000) y = y - 413.34;}//case 0x002B
    //else if (strCmpVal == "3f7f400020365653") {y = x * 500.0 /65536;}//case 0x002C
    //else if (strCmpVal == "3f80000000000000") {y = x * 1.992 / 255;}//case 0x002D
    //else if (strCmpVal == "3f802de00d1b7176") {y = x * 0.0079;}//case 0x002E
    //else if (strCmpVal == "3f84000000000000") {y = x * 640.0 / 0xffff;}//case 0x002F
    //else if (strCmpVal == "3f847ae147ae147b") {y = x * 0.01;}//case 0x0030
    //else if (strCmpVal == "3f8522a6f3f52fc2") {;}//NULL//case 0x0031
    //else if (strCmpVal == "3f8999999999999a") {;}//NULL//case 0x0032
    //else if (strCmpVal == "3f8bf487fcb923a3") {x %= 0x0800;y = x * 0.01365;if (x >= 0x0400) y = y - 27.9552;}//case 0x0033
    //else if (strCmpVal == "3f8c30ab90df5837") {;}//NULL//case 0x0034
    //else if (strCmpVal == "3f90000000000000") {;}//NULL//case 0x0035
    //else if (strCmpVal == "3f90101010101010") {;}//NULL//case 0x0036
    //else if (strCmpVal == "3f901010187bf7c8") {;}//NULL//case 0x0037
    //else if (strCmpVal == "3f90624dd2f1a9fc") {;}//NULL//case 0x0038
    //else if (strCmpVal == "3f93f7ced916872b") {y = x * 0.0195;}//case 0x0039
    //else if (strCmpVal == "3f94000000000000") {y = x * 5.0 / 256.0;}//case 0x003A
    //else if (strCmpVal == "3f940cc78e9f6a94") {;}//NULL//case 0x003B
    //else if (strCmpVal == "3f941205bc01a36e") {y = x * 5.0 / 255.0;}//case 0x003C
    //else if (strCmpVal == "3f94141411b86ccc") {y = x * 5.0 / 255.0;}//case 0x003D
    //else if (strCmpVal == "3f94141414141414") {;}//NULL//case 0x003E
    //else if (strCmpVal == "3f9414a4d2b2bfdb") {;}//NULL//case 0x003F
    //else if (strCmpVal == "3f942c3c9eecbfb1") {y = x * 0.0197;}//case 0x0040
    //else if (strCmpVal == "3f947ae147ae147b") {y = x * 0.02;}//case 0x0041
    //else if (strCmpVal == "3f948f70b8667af6") {;}//NULL//case 0x0042
    //else if (strCmpVal == "3f96161616161616") {;}//NULL//case 0x0043
    //else if (strCmpVal == "3f96872b020c49ba") {y = x * 0.022;}//case 0x0044
    //else if (strCmpVal == "3f96bb98c7e28241") {;}//NULL//case 0x0045
    //else if (strCmpVal == "3f96c16c16c16c17") {;}//NULL//case 0x0046
    //else if (strCmpVal == "3fc2708ede54b48d") {;}//NULL//case 0x0047
    //else if (strCmpVal == "3f989374bc6a7efa") {;}//NULL//case 0x0048
    //else if (strCmpVal == "3f9999999999999a") {y = x * 0.025;}//case 0x0049
    //else if (strCmpVal == "3f9a36e2eb1c432d") {;}//NULL//case 0x004A
    //else if (strCmpVal == "3f9ec2ce4649906d") {;}//NULL//case 0x004B
    //else if (strCmpVal == "3fa0000000000000") {y = x * 2048.0 / 0xffff;}//case 0x004C
    //else if (strCmpVal == "3fa4000000000000") {;}//NULL//case 0x004D
    //else if (strCmpVal == "3fa41205bc01a36e") {y = x * 10.0 / 255;}//case 0x004E
    //else if (strCmpVal == "3fa47ae147ae147b") {y = x * 0.04;}//case 0x004F
    //else if (strCmpVal == "3fa6f0068db8bac7") {y = x * 11.4 / 255;}//case 0x0050
    //else if (strCmpVal == "3fa9000000000000") {;}//NULL//case 0x0052
    //else if (strCmpVal == "3fa916872b020c4a") {y = x * 0.049;}//case 0x0053
    //else if (strCmpVal == "3fa999999999999a") {y = x * 0.05;}//case 0x0054
    //else if (strCmpVal == "3faac5c13fd0d068") {;}//NULL//case 0x0055
    //else if (strCmpVal == "3faccccccccccccd") {y = x * 0.05625;}//case 0x0056
    //else if (strCmpVal == "3fb0000000000000") {y = x * 16.0 / 256;}//case 0x0058
    //else if (strCmpVal == "3fb0624dd2f1a9fc") {y = x * 0.064;}//case 0x0059
    //else if (strCmpVal == "3fb2122fad6cb535") {;}//NULL//case 0x005A
    //else if (strCmpVal == "3fb29c779a6b50b1") {y = x * 18.57 / 255;}//case 0x005C
    //else if (strCmpVal == "3fb2bd3c36113405") {y = x * 0.0732;}//case 0x005D
    //else if (strCmpVal == "3fb4000000000000") {y = x * 19.92 / 255;}//case 0x005E
    //else if (strCmpVal == "3fb41205bc01a36e") {;}//NULL//case 0x005F
    //else if (strCmpVal == "3fb414141603efc6") {y = x * 20.0 / 255;}//case 0x0060
    //else if (strCmpVal == "3fb47ae147ae147b") {y = x * 20.4 / 255;}//case 0x0061
    //else if (strCmpVal == "3fb594af4f0d844d") {y = x * 21.5 / 255;}//case 0x0062
    //else if (strCmpVal == "3fb916872b020c4a") {y = x * 0.098;}//case 0x0063
    //else if (strCmpVal == "3fb999999999999a") {y = x * 0.1;}//case 0x0064
    //else if (strCmpVal == "3fb9db22d0e56042") {y = x * 0.101;}//case 0x0065
    //else if (strCmpVal == "3fbc800000000000") {;}//NULL//case 0x0066
    //else if (strCmpVal == "3fbe000000000000") {;}//NULL//case 0x0067
    //else if (strCmpVal == "3fbf7ced916872b0") {y = x * 0.123;}//case 0x0068
    //else if (strCmpVal == "3fc0000000000000") {y = x * 32.0 / 256.0;}//case 0x0069
    //else if (strCmpVal == "3fc0a3d70a3d70a4") {y = x * 0.13;}
    //else if (strCmpVal == "3fc0b71d83489f56") {y = x * 0.1306;}
    //else if (strCmpVal == "3fc0b71d83ea50b7") {y = x * 0.1306;}//case 0x006C
    //else if (strCmpVal == "3fc16872b020c49c") {;}//NULL//case 0x006D
    //else if (strCmpVal == "3fc1800000000000") {;}//NULL//case 0x006E
    //else if (strCmpVal == "3fc253a07b8b10c0") {y = x * 36.5 / 255;}//case 0x006F
    //else if (strCmpVal == "3fc2708ede54b48d") {;}//NULL//case 0x0070
    //else if (strCmpVal == "3fc851eb851eb852") {;}//NULL//case 0x0071
    //else if (strCmpVal == "3fc8793dd97f62b7") {y = x * 0.1912;}//case 0x0072
    //else if (strCmpVal == "3fc89374bc6a7efa") {;}//NULL//case 0x0073
    //else if (strCmpVal == "3fc89d495182a993") {y = x * 0.1923;}//case 0x0074
    //else if (strCmpVal == "3fc9000000000000") {y = x * 50.0 / 256.0;}//case 0x0075
    //else if (strCmpVal == "3fc999999999999a") {y = x * 51.0 / 255.0;}//case 0x0076
    //else if (strCmpVal == "3fcf3ffac1d29dc7") {;}//NULL//case 0x0077
    //else if (strCmpVal == "3fcf400000000000") {;}//NULL//case 0x0078
    //else if (strCmpVal == "3fd0000000000000") {y = x * 0.25;}//case 0x0079
    //else if (strCmpVal == "3fd0400000000000") {y = x * 0.2539;}//case 0x007A
    //else if (strCmpVal == "3fd0624dd2f1a9fc") {y = x * 65.28 / 255;}//case 0x007B
    //else if (strCmpVal == "3fd072b020c49ba6") {y = x * 0.257;}//case 0x007C
    //else if (strCmpVal == "3fd147ae147ae148") {;}//NULL//case 0x007D
    //else if (strCmpVal == "3fd15cfaacd9e83e") {y = x * 69.5 / 256;}//case 0x007E
    //else if (strCmpVal == "3fd1604189374bc7") {;}//NULL//case 0x007E
    //else if (strCmpVal == "3fd16872b020c49c") {y = x * 8912.9 / 0x8000;if (x >= 0x8000) y = y - 17825.8;}//case 0x0080
    //else if (strCmpVal == "3fd3333333333333") {y = x * 0.3;}//case 0x0081
    //else if (strCmpVal == "3fd3d70a3d70a3d7") {y = x * 0.31;}//case 0x0082
    //else if (strCmpVal == "3fd4d4fdf3b645a2") {;}//NULL//case 0x0083
    //else if (strCmpVal == "3fd4dd2f1a9fbe77") {;}//NULL//case 0x0084
    //else if (strCmpVal == "3fd6666666666666") {;}//NULL//case 0x0085
    //else if (strCmpVal == "3fd6800000000000") {;}//NULL//case 0x0086
    //else if (strCmpVal == "3fd8f5c28f5c28f6") {;}//NULL//case 0x0087
    //else if (strCmpVal == "3fd9000000000000") {y = x * 100.0 / 255.0;}//case 0x0088
    //else if (strCmpVal == "3fd90624dd2f1aa0") {;}//NULL//case 0x0089
    //else if (strCmpVal == "3fd916872b020c4a") {y = x * 100.0 / 255.0;}//case 0x008A
    //else if (strCmpVal == "3fd9191919191919") {y = x * 100.0 / 255.0;}//case 0x008B
    //else if (strCmpVal == "3fd91919195f2a3b") {y = x * 100.0 / 255.0;}//case 0x008C
    //else if (strCmpVal == "3fd91919ac79702e") {y = x * 100.0 / 255.0;}//case 0x008D
    //else if (strCmpVal == "3fd999999999999a") {y = x * 0.4;}//case 0x008E
    //else if (strCmpVal == "3fdc28f5c28f5c29") {;}//NULL//case 0x008F
    //else if (strCmpVal == "3fdd180000000000") {;}//NULL//case 0x0090
    //else if (strCmpVal == "3fdf400000000000") {y = x * 125.0 / 255.0;}//case 0x0091
    //else if (strCmpVal == "3fe0000000000000") {y = x * 0.5;}//case 0x0092
    //else if (strCmpVal == "3fe10624dd2f1aa0") {;}//NULL//case 0x0093
    //else if (strCmpVal == "3fe1919191919192") {y = x * 140.0 / 255.0;}//case 0x0094
    //else if (strCmpVal == "3fe4000000000000") {y = x * 159.0 / 255.0;}//case 0x0095
    //else if (strCmpVal == "3fe4c80000000000") {;}//NULL//case 0x0096
    //else if (strCmpVal == "3fe547ae147ae148") {;}//NULL//case 0x0097
    //else if (strCmpVal == "3fe9000000000000") {y = x * 0.78125;}//case 0x0098
    //else if (strCmpVal == "3fe93264c993264d") {;}//NULL//case 0x0099
    //else if (strCmpVal == "3fe999999999999a") {y = x * 0.8;}//case 0x009A
    //else if (strCmpVal == "3feccccccccccccd") {y = x * 0.9;}//case 0x009B
    //else if (strCmpVal == "3ff0000000000000") {y = x;}//case 0x009C
    //else if (strCmpVal == "3ff0624dd2f1a9fc") {y = x * 67108.0 / 65535.0;}//case 0x009D
    //else if (strCmpVal == "3ff0a00000000000") {;}//NULL//case 0x009E
    //else if (strCmpVal == "3ff0cccccccccccd") {y = x * 1.05;}//case 0x009F
    //else if (strCmpVal == "3ff10624dd2f1aa0") {;}//NULL//case 0x00A0
    //else if (strCmpVal == "3ff160aa64c2f838") {;}//NULL//case 0x00A1
    //else if (strCmpVal == "3ff160bf5d78811b") {y = x * 277.0 / 255.0;}//case 0x00A2
    //else if (strCmpVal == "3ff47ae147ae147b") {y = x * 1.28;}//case 0x00A3
    //else if (strCmpVal == "3ff4cccccccccccd") {y = x * 1.3;}//case 0x00A4
    //else if (strCmpVal == "3ff4e978d4fdf3b6") {y = x * 1.307;}//case 0x00A5
    //else if (strCmpVal == "3ff610624dd2f1aa") {y = x * 351.6 / 255;}//case 0x00A6
    //else if (strCmpVal == "3ff7851eb851eb85") {y = x * 1.47;}
    //else if (strCmpVal == "3ff8000000000000") {;}//NULL//case 0x00A8
    //else if (strCmpVal == "3ff999999999999a") {y = x * 1.6;}
    //else if (strCmpVal == "3ffb851eb851eb85") {y = x * 1.72;}
    //else if (strCmpVal == "4000000000000000") {y = x * 2.0;}//case 0x00AB
    //else if (strCmpVal == "40039eb851eb851f") {;}//NULL//case 0x00AC
    //else if (strCmpVal == "4004000000000000") {;}//NULL//case 0x00AD
    //else if (strCmpVal == "4008000000000000") {y = x * 3.0;}
    //else if (strCmpVal == "4008c00000000000") {y = x * 3.09375;}
    //else if (strCmpVal == "400999999999999a") {;}//NULL
    //else if (strCmpVal == "4014000000000000") {y = x * 5.0;}//case 0x00B1
    //else if (strCmpVal == "4014008bcf64e5ec") {y = x * 327710.0 / 0xffff;}//case 0x00B2
    //else if (strCmpVal == "4020000000000000") {y = x * 8.0;}//case 0x00B3
    //else if (strCmpVal == "4024000000000000") {y = x * 10.0;}//case 0x00B4
    //else if (strCmpVal == "402e000000000000") {;}//NULL//case 0x00B5
    //else if (strCmpVal == "4034000000000000") {;}//NULL//case 0x00B6
    //else if (strCmpVal == "4039000000000000") {;}//NULL//case 0x00B7
    //else if (strCmpVal == "403f400000000000") {;}//NULL//case 0x00B8
    //else if (strCmpVal == "4040000000000000") {y = x * 32.0;}//case 0x00B9
    //else if (strCmpVal == "4049000000000000") {y = x * 50.0;}//case 0x00BA
    //else if (strCmpVal == "4051c00000000000") {;}//NULL//case 0x00BB
    //else if (strCmpVal == "4052c00000000000") {y = x * 75.0;}//case 0x00BC
    //else if (strCmpVal == "4059000000000000") {y = x * 100.0;}//case 0x00BD
    //else if (strCmpVal == "bfab51eb851eb852") {;}//NULL//case 0x00BE
    //else if (strCmpVal == "bfd4d555318abc87") {;}//NULL//case 0x00BF
    //else if (strCmpVal == "bfe1604189374bc7") {;}//NULL//case 0x00C0
    //else if (strCmpVal == "bff0000000000000") {;}//NULL//case 0x00C1
    //else if (strCmpVal == "c059000000000000") {y = x * 180.0 / 255;}//case 0x00C2

    return y;
}

/* SubaruData  数据流算法处理 */
string CSubaruBaseCanApp::SubaDataCanCal(CBinary binAns, vector<string> ListData)
{
    string strRet = "";

    float y = 0;
    W_U32 x = 0;
    W_I16 startPos, NeedLen;
    LONG64 CaltempValue = 0x01;
    startPos = StringToHex(ListData[5]);
    NeedLen = StringToHex(ListData[6]);

    if (binAns.GetByteCount() < startPos + NeedLen)
        x = 0;
    else
    {
        for (W_I16 i = 0; i < NeedLen; i++)
        {
            if (binAns.GetByteCount() > startPos + i)
                x += binAns[startPos + i] << ((NeedLen - i - 1) * 8);
            else
                x += 0;
            CaltempValue = CaltempValue << 8;
        }
    }
    string format = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(ListData[9]));

    // 如果第十列存在算法
    if ("000000b1" == ListData[10])
    {
        strRet = "";
        for (W_I16 i = 0; i < NeedLen; i++)
        {
            if (binAns.GetByteCount() > startPos + i)
                strRet += binAns[startPos + i];
            else
                strRet = "";
        }
        return strRet;
    }
    else if ("000000b2" == ListData[10] || "00000020" == ListData[10])
    {
        char buf[128];
        if ("00000020" == ListData[10] && x >= 0x80000000)
            x = 0;
        sprintf(buf, "%X", x);
        strRet = buf;
        return strRet;
    }
    else if ("000007fe" == ListData[10])
    {
        strRet = "0";
        if (3 > NeedLen)
            return strRet;
        W_I16 BYTE0 = (x & 0xff000000) >> 24;
        W_I16 BYTE1 = (x & 0xff0000) >> 16;
        W_I16 BYTE3 = x & 0xff;
        if (0 != BYTE3)
        {
            y = 0;
            if (0xff == BYTE0)
                y = ((0xff0000 - x * 0x20) & 0xff) * 0x1000000;
            else if (0xfe == BYTE0)
                y = ((0xff0000 - x * 0x10) & 0xff) * 0x1000000;
            else if (0xfd == BYTE0)
                y = ((0xff0000 - x * 8) & 0xff) * 0x1000000;
            else if (BYTE0 > 0xf7)
                y = ((0xff0000 - x * 4) & 0xff) * 0x1000000;
            else if (BYTE0 > 0xef)
                y = ((0xff0000 - x * 2) & 0xff) * 0x1000000;
            else if (BYTE0 > 0xDF)
                y = ((0xff0000 - x) & 0xff) * 0x1000000;
            else if (BYTE0 > 0xBF)
                y = ((0xff0000 - x * 2) & 0x0fff) * 0x100000;
            else if (BYTE0 > 0x7f)
                y = ((0xff0000 - x * 4) & 0x0fff) * 0x100000;
            else if (BYTE0 > 0x3f)
                y = x * 4 * 0x100000;
            else if (BYTE0 > 0x1f)
                y = x * 8 * 0x100000;
            else if (BYTE0 > 0x0f)
                y = x * 0x1000000;
            else if (BYTE0 > 0x07)
                y = ((x * 2) & 0xff) * 0x1000000;
            else if (BYTE0 > 0x03)
                y = ((x * 4) & 0xff) * 0x1000000;
            else if (BYTE0 > 0x00)
                y = ((x * 8) & 0xff) * 0x1000000;
            else if (BYTE1 > 0x7f)
                y = x % 8 * 0x20000000;
            else if (BYTE1 > 0x3f)
                y = x % 4 * 0x40000000;
            else if (BYTE1 > 0x1f)
                y = x % 2 * 0x80000000;

            char buf[24];
            sprintf(buf, "%X", (LONG64)y);
            strRet = buf;
        }
        return strRet;
    }
    else if ("000007d6" == ListData[10])
    {
        strRet = "0";
        return strRet;
    }
    else if ("00000064" == ListData[10] || "00000065" == ListData[10] || "00000066" == ListData[10] || "00000067" == ListData[10]
        || "00000068" == ListData[10] || "00000069" == ListData[10] || "0000006a" == ListData[10] || "0000006b" == ListData[10]
        || "0000006c" == ListData[10] || "0000006f" == ListData[10] || "00000081" == ListData[10] || "00000082" == ListData[10]
        || "00000083" == ListData[10] || "00000087" == ListData[10])
    {
        W_I32 IntTemp1 = StringToHex(ListData[17]);
        char buf[128];
        x &= IntTemp1;
        switch (StringToHex(ListData[10]))
        {
        case 0x00000064: y = x * 100.0 / 255; break;
        case 0x00000065: y = x - 40; break;
        case 0x00000066: y = x * 200.0 / 256 - 100; break;
        case 0x00000067: y = x * 3; break;
        case 0x00000068: y = x; break;
        case 0x00000069: y = x * 0.25; break;
        case 0x0000006a: y = x * 0.5 - 64.0; break;
        case 0x0000006b: y = x * 0.01; break;
        case 0x0000006c: y = x * 0.005; break;
        case 0x0000006f: if (x & 0x7f) y = 1; else y = 0; break;
        case 0x00000081: y = x * 2.0 / 65535; break;
        case 0x00000082: y = x * 8.0 / 65535; break;
        case 0x00000083: y = x * 256.0 / 65535 - 128; break;
        case 0x00000087: y = x * 15.0 / 255; break;
        }

        y += CalList15(ListData[15]);
        W_U8 rightNum = 0;
        bool iftoa = false;
        for (W_I16 i = 0; i < format.length(); i++)
        {
            if ('.' == format.at(i) && i + 1 < format.length())
            {
                iftoa = true;
                rightNum = format.at(i + 1) - 0x30;
                break;
            }
        }
        if (iftoa)
        {
            strRet = ftoa(y, buf, rightNum);
        }
        else
        {
            sprintf(buf, format.c_str(), y);
            strRet = buf;
        }
        return strRet;
    }
    else if ("000000bf" == ListData[10] || "000000c0" == ListData[10] || "000000c1" == ListData[10])
    {
        char buf[3];
        for (W_I16 i = NeedLen - 1; i >= 0; i--)
        {
            if (binAns.GetByteCount() > startPos + i)
            {
                W_U8 iTemp = HextoInt(binAns[startPos + i]);
                if (0 != iTemp)
                {
                    sprintf(buf, "%02d", iTemp);
                    strRet += buf;
                }
            }
        }
        return strRet;
    }
    else if ("000000c2" == ListData[10])
    {
        W_I16 BYTE0 = x & 0x0f;
        W_I16 BYTE1 = HextoInt((x & 0xff00) >> 8);
        if (BYTE0 >= 0x0A)
            BYTE0 = 0;

        char buf[5];
        sprintf(buf, "%1.2f", BYTE0 + (BYTE1 / 100.0));
        strRet = buf;
        return strRet;
    }

    vector<string> vecStr;
    vecStr.push_back(ListData[12]);    // 状态类型算法
    vecStr.push_back(ListData[14]);    // x * XX
    vecStr.push_back(ListData[15]);    // x +/-= XX
    vecStr.push_back(ListData[16]);    // x /= XX
    vecStr.push_back(ListData[17]); // x &= XX
    vecStr.push_back(ListData[19]);

    // Switch格式 调用文件SubaDataBtn.ctv
    if ("00000000" != vecStr[0])
    {
        if (vecStr[0] == "00220156")  //固定值 返回"北"
            return FxGetStdString(CBinary("\x53\x4D\xD7\x7F\x16\x20", 6));
        if (binAns.GetByteCount() <= startPos)
            return "";
        W_I32 IntTemp1 = StringToHex(vecStr[3]), IntTemp2;
        switch (IntTemp1)
        {
        case 0xfffffff9:    x /= 0x80; break;
        case 0xfffffffa:    x /= 0x40; break;
        case 0xfffffffb:    x /= 0x20; break;
        case 0xfffffffc:    x /= 0x10; break;
        case 0xfffffffd:    x /= 0x08; break;
        case 0xfffffffe:    x /= 0x04; break;
        case 0xffffffff:    x /= 0x02; break;
        }
        IntTemp1 = StringToHex(vecStr[4]);
        x &= IntTemp1;

        vector<vector<string>> SearchResault;
        if (0 < DataStatus.size())
        {
            SearchResault = DataStatus[StringToHex(vecStr[0])];
        }
        if (0 == DataStatus.size() || 0 == SearchResault.size())
        {
            // 只打开一次该文件,将内容都保存在DataStatus中
            CSearchString SubaDataBtn;
            vector<string> vecTemp;
            if (FALSE == SubaDataBtn.OpenTabFile(Subaru_Data_Btn))
                return strRet;
            vecTemp.push_back(ListData[0]);
            SubaDataBtn.SearchString(SearchResault, FALSE, 0, 0, vecTemp);
            for (W_I16 i = 0; i < SearchResault.size(); i++)
                DataStatus[StringToHex(SearchResault[i][1])].push_back(SearchResault[i]);
            SearchResault = DataStatus[StringToHex(vecStr[0])];
        }

        for (W_I16 i = 0; i < SearchResault.size(); i++)
        {
            if ("40000000" != SearchResault[i][2])
            {
                // 如果 x 与 第三列等于第四列 显示第5列的值
                IntTemp1 = StringToHex(SearchResault[i][3]);
                IntTemp2 = StringToHex(SearchResault[i][4]);
                if ((x & IntTemp1) == IntTemp2)
                {
                    strRet = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(SearchResault[i][5]));
                    break;
                }
            }
            else    // "40000000" 即default
            {
                strRet = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(SearchResault[i][5]));
            }
        }
    }
    else    //第14列及15列算法
    {
        if ("00000089" == ListData[10])//
        {
            W_I32 IntTemp1 = StringToHex(vecStr[4]);
            x &= IntTemp1;
        }
        if ("0000000a" == ListData[10] && x >= CaltempValue)
        {
            if ("00000004" != SystemNum)//if ("DBFT3000" != SystemNum)
                x -= CaltempValue;
        }

        char buf[128];

        if ((ListData[14] == "3fb999999999999a" || ListData[14] == "3ff0000000000000" || ListData[14] == "4059000000000000" || ListData[14] == "3f5c9870be72dbaf"
            || ListData[14] == "3f847ae147ae147b") && ListData[6] == "00000004")
        {
            LONG64 iValue = 0;
            if (ListData[14] == "3fb999999999999a")
            {
                iValue = (LONG64)(x * 0.1);
                sprintf(buf, format.c_str(), iValue * 1.0);
            }
            else if (ListData[14] == "4059000000000000")
            {
                iValue = (LONG64)(x * 100.0);
                sprintf(buf, format.c_str(), iValue * 1.0);
            }
            else if (ListData[14] == "3ff0000000000000")
            {
                iValue = x;
                if (ListData[15] == "c049000000000000")
                    iValue -= 50;
                sprintf(buf, format.c_str(), iValue * 1.0);
            }
            else if (ListData[14] == "3f847ae147ae147b")
            {
                iValue = (LONG64)(x * 0.01);
                sprintf(buf, format.c_str(), iValue * 1.0);
            }
            else if (ListData[14] == "3f5c9870be72dbaf")
            {
                iValue = (LONG64)(x * 1.745329);
                sprintf(buf, format.c_str(), iValue * 1.0 / 1000.0);
            }

            strRet = buf;
            return strRet;
        }

        y = SubaDataCanCal_getYValue(vecStr, x);

        W_U8 rightNum = 0;
        bool iftoa = false;
        for (W_I16 i = 0; i < format.length(); i++)
        {
            if ('.' == format.at(i) && i + 1 < format.length())
            {
                iftoa = true;
                rightNum = format.at(i + 1) - 0x30;
                break;
            }
        }
        if (iftoa)
        {
            strRet = ftoa(y, buf, rightNum);
        }
        else
        {
            sprintf(buf, format.c_str(), y);
            strRet = buf;
        }
    }

    return strRet;
}

W_ErrorCode CSubaruBaseCanApp::ActiveTest()
{
    W_ErrorCode    rect = CErrorCode::EC_SUCCESS;


    return rect;
}

void CSubaruBaseCanApp::SpecFunsIDSet()
{
    SpecFunsID.push_back("028f4f20");        // 更换DPF  更换
    SpecFunsID.push_back("028e4f20");        // 更新DPF     再生
    SpecFunsID.push_back("028d4f20");        //更换机油
    SpecFunsID.push_back("028c4f20");        //读取DPF／机油相关的训练值。ECM -> TOOL
    SpecFunsID.push_back("028b4f20");        //写入DPF／机油相关的训练值。TOOL -> ECM
    SpecFunsID.push_back("02674f20");        //新注册喷油器代码（TOOL至ECM）    
    SpecFunsID.push_back("02664f20");        //读取喷油器代码（ECM至TOOL）
    SpecFunsID.push_back("02654f20");        //注册正在读取的代码（TOOL至ECM）
    SpecFunsID.push_back("02644f20");        //喷油器代码显示
    SpecFunsID.push_back("02071120");        //登入VIN    
    SpecFunsID.push_back("02871c20");        //燃油泵继电器
    SpecFunsID.push_back("03871c20");        //CPC 电磁阀
    SpecFunsID.push_back("00871c20");        //散热器风扇继电器
    SpecFunsID.push_back("01871c20");        //空调压缩机继电器
    SpecFunsID.push_back("04871c20");        //PCV 电磁阀
    SpecFunsID.push_back("05871c20");        //排气控制电磁阀
    SpecFunsID.push_back("09871c20");        //油箱内力交换电磁阀
    SpecFunsID.push_back("0e871c20");        //涡轮增压器废气口电磁线圈
    SpecFunsID.push_back("0c871c20");        //辅助空气组合阀 1
    SpecFunsID.push_back("0d871c20");        //辅助空气组合阀 2
    SpecFunsID.push_back("12871c20");        //辅助气泵继电器
    SpecFunsID.push_back("13871c20");        //CPC电磁阀2
    SpecFunsID.push_back("10871c20");        //辅助燃油泵继电器
    SpecFunsID.push_back("11871c20");        //燃油压切换电磁阀
    SpecFunsID.push_back("16871c20");        //ELCM开关阀
    SpecFunsID.push_back("17871c20");        //ELCM泵
    SpecFunsID.push_back("14871c20");        //刹车真空泵继电器    
    SpecFunsID.push_back("02071a20");        //防盗锁系统        
    SpecFunsID.push_back("02774f20");        //燃油泵能率记忆
    SpecFunsID.push_back("02764f20");        //燃油喷射器喷射量记忆
    SpecFunsID.push_back("02754f20");        //EGR阀门开角记忆
    SpecFunsID.push_back("02374c20");        //涡轮叶片角记忆
    SpecFunsID.push_back("02274c20");        //气流传感器记忆
    SpecFunsID.push_back("12637620");        //清除AT记忆值
    SpecFunsID.push_back("42637620");        //AWD 开/关切换模式
    SpecFunsID.push_back("02051b20");        //ABS 序列控制模式
    SpecFunsID.push_back("02041b20");        //VDC 检查模式
    SpecFunsID.push_back("02061b20");        //VSC（VDC）中间值设置模式
    SpecFunsID.push_back("02021b20");        //刹车维护模式    
    SpecFunsID.push_back("d2647620");        //模式移动    
    SpecFunsID.push_back("42667620");        //无钥匙 ID 注册
    SpecFunsID.push_back("32667620");        //擦除无钥ID码    
    SpecFunsID.push_back("c2607620");        //重新调零


    if (SystemNum == "0000001A")
    {
        SpecFunsID.push_back("01844220");        //怠速控制                    
        SpecFunsID.push_back("00844220");        //固定怠速点火定时            
    }
    SpecFunsID.push_back("22637620");        //自动变速器记忆模式                
    //SpecFunsID.push_back("62667620");        //SCU强制校对模式                

    SpecFunsID.push_back("02a64f20");        //力传感器校准模式      
    SpecFunsID.push_back("02a54f20");        //停车刹车的违章驾驶模式
    SpecFunsID.push_back("02a44f20");        //停车刹车拆卸模式      
    SpecFunsID.push_back("02a24f20");        //离合器结合位置设定    
    SpecFunsID.push_back("02a14f20");        //离合器传感器校准模式  
    SpecFunsID.push_back("02a34f20");        //参数初始化模式        
}

W_ErrorCode CSubaruBaseCanApp::SpecialFunction()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    if (SpecfunsSet.size() >= 1 && SpecfunsSet[0].size() > 1)
    {
        vector<string> SelectedInfo;
        SelectedInfo.push_back(SpecfunsSet[0][0]);
        ShowSepcialFunMenu(SelectedInfo);
    }
    else
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_THE_SELECTED_FUNCTION_IS_NOT_SUPPORTED, DF_MB_OK, DT_LEFT);

    return rect;
}

W_ErrorCode CSubaruBaseCanApp::ShowSepcialFunMenu(vector<string> SelectedInfo)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binTemp;
    string strTemp;
    CMenuCtrl Menu;
    vector<vector<string> > SpecFunsSetTemp;
    vector<string> SelectTemp;
    for (W_I16 i = 0; i < SpecfunsSet.size(); i++)
    {
        bool flag = true;
        for (W_I16 j = 0; j < SelectedInfo.size() && flag; j++)
        {
            if (SpecfunsSet[i].size() <= SelectedInfo.size() || !CompareStringIsEquation(SpecfunsSet[i][j], SelectedInfo[j]))
                flag = false;
        }
        if (flag)
        {
            SpecFunsSetTemp.push_back(SpecfunsSet[i]);
            if (SelectedInfo.size() == SpecfunsSet[i].size() - 1)    // 调用特殊功能ID
            {
                binTemp = CBinary(Subaru_IDCAN, 2) + String2Binary(SelectedInfo[SelectedInfo.size() - 1]);
                W_I32 taskid = StringToHex(SpecfunsSet[i][SpecfunsSet[i].size() - 1]);
                return SpecialFunsCalID(binTemp, taskid);
            }
            for (W_I16 j = 0; j < SelectTemp.size(); j++)
            {
                if (SelectTemp[j] == SpecfunsSet[i][SelectedInfo.size()])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
                SelectTemp.push_back(SpecfunsSet[i][SelectedInfo.size()]);
        }
    }
    while (1)
    {
        Menu.InitCtrl(CBinary(Subaru_IDCAN, 2) + String2Binary(SelectedInfo[SelectedInfo.size() - 1]));
        for (W_I16 i = 0; i < SelectTemp.size(); i++)
        {
            strTemp = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(SelectTemp[i]));
            if (strTemp.length() > 0)
                Menu.AddOneItem(strTemp);
            else
                SelectTemp.erase(SelectTemp.begin() + i--);
        }
        W_I16 iSelect = Menu.ShowCtrl();
        if (-1 == iSelect)
            break;
        vector<string> vecStrTemp = SelectedInfo;
        vecStrTemp.push_back(SelectTemp[iSelect]);
        ShowSepcialFunMenu(vecStrTemp);
    }

    return rect;
}



W_ErrorCode CSubaruBaseCanApp::SpecialFunsCalID(CBinary binMenu, W_I32 taskid)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    DataStatus.clear();    // 一些特殊功能使用的数据流并非其本来系统的

    switch (taskid)
    {
    case 0x028f4f20: rect = SpecFunsID_028f4f20(binMenu); break;        // 更换DPF                              
    case 0x028e4f20: rect = SpecFunsID_028e4f20(binMenu); break;        // 更新DPF                              
    case 0x028d4f20: rect = SpecFunsID_028d4f20(binMenu); break;        //更换机油                              
    case 0x028c4f20: rect = SpecFunsID_028c4f20(binMenu); break;        //读取DPF／机油相关的训练值。ECM -> TOOL
    case 0x028b4f20: rect = SpecFunsID_028b4f20(binMenu); break;        //写入DPF／机油相关的训练值。TOOL -> ECM
    case 0x02674f20: rect = SpecFunsID_02674f20(binMenu); break;        //新注册喷油器代码（TOOL至ECM）         
    case 0x02664f20: rect = SpecFunsID_02664f20(binMenu); break;        //读取喷油器代码（ECM至TOOL）           
    case 0x02654f20: rect = SpecFunsID_02654f20(binMenu); break;        //注册正在读取的代码（TOOL至ECM）       
    case 0x02644f20: rect = SpecFunsID_02644f20(binMenu); break;        //喷油器代码显示                        
    case 0x02071120: rect = SpecFunsID_02071120(binMenu); break;        //登入VIN                               
    case 0x02871c20: rect = SpecFunsID_02871c20(binMenu); break;        //燃油泵继电器                          
    case 0x03871c20: rect = SpecFunsID_03871c20(binMenu); break;        //CPC 电磁阀                            
    case 0x00871c20: rect = SpecFunsID_00871c20(binMenu); break;        //散热器风扇继电器                      
    case 0x01871c20: rect = SpecFunsID_01871c20(binMenu); break;        //空调压缩机继电器                      
    case 0x04871c20: rect = SpecFunsID_04871c20(binMenu); break;        //PCV 电磁阀                            
    case 0x05871c20: rect = SpecFunsID_05871c20(binMenu); break;        //排气控制电磁阀                        
    case 0x09871c20: rect = SpecFunsID_09871c20(binMenu); break;        //油箱内力交换电磁阀                    
    case 0x0e871c20: rect = SpecFunsID_0e871c20(binMenu); break;        //涡轮增压器废气口电磁线圈              
    case 0x0c871c20: rect = SpecFunsID_0c871c20(binMenu); break;        //辅助空气组合阀 1                      
    case 0x0d871c20: rect = SpecFunsID_0d871c20(binMenu); break;        //辅助空气组合阀 2                      
    case 0x12871c20: rect = SpecFunsID_12871c20(binMenu); break;        //辅助气泵继电器                        
    case 0x13871c20: rect = SpecFunsID_13871c20(binMenu); break;        //CPC电磁阀2                            
    case 0x10871c20: rect = SpecFunsID_10871c20(binMenu); break;        //辅助燃油泵继电器                      
    case 0x11871c20: rect = SpecFunsID_11871c20(binMenu); break;        //燃油压切换电磁阀                      
    case 0x16871c20: rect = SpecFunsID_16871c20(binMenu); break;        //ELCM开关阀                            
    case 0x17871c20: rect = SpecFunsID_17871c20(binMenu); break;        //ELCM泵                                
    case 0x14871c20: rect = SpecFunsID_14871c20(binMenu); break;        //刹车真空泵继电器                      
    case 0x02071a20: rect = SpecFunsID_02071a20(binMenu); break;        //防盗锁系统                            
    case 0x02774f20: rect = SpecFunsID_02774f20(binMenu); break;        //燃油泵能率记忆                        
    case 0x02764f20: rect = SpecFunsID_02764f20(binMenu); break;        //燃油喷射器喷射量记忆                  
    case 0x02754f20: rect = SpecFunsID_02754f20(binMenu); break;        //EGR阀门开角记忆                       
    case 0x02374c20: rect = SpecFunsID_02374c20(binMenu); break;        //涡轮叶片角记忆                        
    case 0x02274c20: rect = SpecFunsID_02274c20(binMenu); break;        //气流传感器记忆  
    case 0x01844220: rect = SpecFunsID_01844220(binMenu); break;        //怠速控制                
    case 0x00844220: rect = SpecFunsID_00844220(binMenu); break;        //固定怠速点火定时
    case 0x22637620: rect = SpecFunsID_22637620(binMenu); break;        //自动变速器记忆模式
    case 0x12637620: rect = SpecFunsID_12637620(binMenu); break;        //清除AT记忆值
    case 0x42637620: rect = SpecFunsID_42637620(binMenu); break;        //AWD 开/关切换模式    
    case 0x02061b20: rect = SpecFunsID_02061b20(binMenu); break;        //VSC（VDC）中间值设置模式
    case 0x02051b20: rect = SpecFunsID_02051b20(binMenu); break;        //ABS 序列控制模式
    case 0x02041b20: rect = SpecFunsID_02041b20(binMenu); break;        //VDC 检查模式
    case 0x02021b20: rect = SpecFunsID_02021b20(binMenu); break;        //刹车维护模式
    case 0xd2647620: rect = SpecFunsID_d2647620(binMenu); break;        //模式移动
    case 0x32667620: rect = SpecFunsID_32667620(binMenu); break;        //擦除无钥ID码
    case 0x42667620: rect = SpecFunsID_42667620(binMenu); break;        //无钥匙 ID 注册
    case 0xc2607620: rect = SpecFunsID_c2607620(binMenu); break;        //重新调零

    case 0x02a64f20: rect = SpecFunsID_02a64f20(binMenu); break;        //力传感器校准模式
    case 0x02a54f20: rect = SpecFunsID_02a54f20(binMenu); break;        //停车刹车的违章驾驶模式
    case 0x02a44f20: rect = SpecFunsID_02a44f20(binMenu); break;        //停车刹车拆卸模式
    case 0x02a24f20: rect = SpecFunsID_02a24f20(binMenu); break;        //离合器结合位置设定
    case 0x02a14f20: rect = SpecFunsID_02a14f20(binMenu); break;        //离合器传感器校准模式
    case 0x02a34f20: rect = SpecFunsID_02a34f20(binMenu); break;        //参数初始化模式
    }

    return rect;
}

// 更换DPF
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_028f4f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    // 0x53200000003E        "是否更换了柴油颗粒滤清器（DPF）？"
    // 0x53200000003F        "已重置灰尘堆积速率和烟尘堆积速率。"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x3E", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x3F", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;

    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31018000("\x31\x01\x80\x00", 4);
    CBinary bin31038000("\x31\x03\x80\x00", 4);
    CBinary binAns;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31018000, binAns)))
        return rect;

    W_I16 iRet = 0;
    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            binAns.Clear();
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31038000, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() < 6)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return 0;
        }
        // 0x53200000004B        "无法重置灰尘和烟尘堆积速率"
        // 0x532000000040        "当前数据显示/保持模式可用于确认已经重置灰尘堆积速率和烟尘堆积速率。"
        if (0x00 == binAns[4])
        {
            if (0x10 == binAns[5])
            {
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x4B", 6), DF_MB_OK, DT_LEFT);
                break;
            }
            else if (0x02 == binAns[5])
            {
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x40", 6), DF_MB_OK, DT_LEFT);
                break;
            }
            else if (DF_IDCANCEL == (iRet = FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT)))
            {
                return rect;
            }
        }
        else if (DF_IDCANCEL == (iRet = FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT)))
        {
            return rect;
        }
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 更新DPF
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_028e4f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    string strTemp;
    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin221031("\x22\x10\x31", 3);
    CBinary bin221030("\x22\x10\x30", 3);
    CBinary bin31018002("\x31\x01\x80\x02", 4);
    CBinary bin31038002("\x31\x03\x80\x02", 4);
    CBinary bin31028002("\x31\x02\x80\x02", 4);
    CBinary bin22114D("\x22\x11\x4d", 3);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221031, binAns)))
        return rect;
    if (binAns.GetByteCount() > 3 && 0x00 != binAns[3])
    {
        // 0x532000000041        "烟尘堆积速率过高，导致柴油颗粒滤清器（DPF）无法再生"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x41", 6), DF_MB_OK, DT_CENTER);
        return rect;;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221030, binAns)))
        return rect;
    if (binAns.GetByteCount() >= 5 && 0xff == binAns[3])
    {
        // 0x5320000000C2        "柴油颗粒滤清器（DPF）再生耗时过久（通常超过1小时）"
        // 0x5320000000C3        "进行柴油颗粒滤清器（DPF）再生"
        if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC2", 6), DF_MB_OKCANCEL, DT_CENTER))
            return rect;
        if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC3", 6), DF_MB_OKCANCEL, DT_CENTER))
            return rect;
    }
    // 0x5320000000C0        "烟尘堆积速率过低，因此柴油颗粒滤清器（DPF）没有必要再生。\n是否强制再生？"
    // 0x5320000000BB        "拉起驻车制动器。"
    // 0x5320000000C1        "请留意实施过程中发动机转速的变化情况。\n再生完成之前请留在车内。"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC0", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xBB", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC1", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31018002, binAns)))
        return rect;

    DWORD iTimer = 0;
    W_I16 iCountdown = 0;
    string iValue;
    CBinary binUnit("\x53\x20\x00\x00\x00\xFA", 6);
    binAns.Clear();
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31038002, binAns)))
                return rect;
            if (binAns.GetByteCount() < 8)
            {
                FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_ECU_REFUSE;
            }
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x10 == binAns[5] || 0x02 == binAns[5]))
        {
            if (0x02 == binAns[5])
            {
                iCountdown = 660;
                break;
            }
            if (0x01 == binAns[5])
            {
                // 0x5320000000BD        "柴油颗粒滤清器（DPF）再生过程中的\n烟尘堆积速率 "
                strTemp = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xBD", 6));
                char buf[4];
                iTimer = 0;
                while (1)
                {
                    if (iTimer < GetTickCount())
                    {
                        iTimer = GetTickCount() + 150;
                        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin22114D, binAns)))
                            return rect;
                        iValue = "";
                        if (binAns.GetByteCount() > 3)
                            iValue = itoa(binAns[3], buf, 10);
                        else
                            iValue += 0x31;
                        iValue += FxGetStdString(binUnit);
                    }
                    if (DF_IDCANCEL == FxShowMessageBox(FxGetStdString(binMenu), strTemp + iValue, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
                    {
                        // "DPF更新停止。\n如要重新再生，关闭点火，稍等15秒。"
                        string strShow = FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x2C", 6)) + "\n" + FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x2D", 6));
                        if (DF_IDOK == FxShowMessageBox(FxGetStdString(binMenu), strShow, DF_MB_OKCANCEL, DT_LEFT))
                            return rect;
                    }
                }
            }
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31028002, binAns)))
        return rect;
    m_pNetLayer->SendReceive(ExitCmd);

    binUnit = CBinary("\x53\x4D\x0F\x5D\x09\x73", 6);//min
    strTemp = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xBA", 6));
    string maohao = ":";
    char buf[3];
    CMessageBoxCtrl::SetBusyStatus(true);//等待10分钟 加个漏斗
    while (iCountdown--)
    {
        sprintf(buf, strTemp.c_str(), iCountdown / 60);
        iValue = buf;
        FxShowMessageBox(FxGetStdString(binMenu), iValue, DF_MB_NOBUTTON, DT_CENTER);// 0x5320000000BA        "正在冷却催化器，请稍等。\n剩余时间是"
        Sleep(1000);
        if (0 == iCountdown)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xBF", 6), DF_MB_OK, DT_CENTER);//"柴油颗粒滤清器（DPF）已完成再生。"
            break;
        }
    }
    CMessageBoxCtrl::SetBusyStatus(false);

    return rect;
}

// 更换机油
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_028d4f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x532000000043        "是否更换了发动机机油？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x43", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x44", 6), DF_MB_OKCANCEL, DT_CENTER))//机油稀释率已重置
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;

    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31018001("\x31\x01\x80\x01", 4);
    CBinary bin31038001("\x31\x03\x80\x01", 4);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31018001, binAns)))
        return rect;

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31038001, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x02 == binAns[5])
        {
            // 0x532000000045        "机油稀释率已重置为正常比率"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x45", 6), DF_MB_OK, DT_CENTER);
            break;
        }
        else if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
        {
            // 0x532000000049        "无法重置机油稀释率"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x49", 6), DF_MB_OK, DT_CENTER);
            return rect;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
    }
    // 0x532000000046        "确认是否已经在'当前数据显示和保存'中重置了机油稀释率"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x46", 6), DF_MB_OK, DT_CENTER);

    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 从Tempdata.dat文件里取出数据
W_ErrorCode CSubaruBaseCanApp::InforReadfromFile(vector<string>& iData, string List0)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CFile fileWrite;
    CString strFileNamePath = GetExePath();                    // 当前exe路径
    strFileNamePath += _T(Subaru_Special_DAT);    // 文件名

    string strTempFileData;
    /****************************** 读 ****************************/
    if (fileWrite.Open(strFileNamePath, CFile::modeRead))
    {
        int nBmpLeng = fileWrite.GetLength();
        if (nBmpLeng > 0)
        {
            char* szBuffer;
            szBuffer = new char[nBmpLeng + 1];
            memset(szBuffer, 0x00, nBmpLeng + 1);
            ASSERT(szBuffer);
            fileWrite.Read(szBuffer, nBmpLeng);
            string strTempFile(szBuffer);
            strTempFileData = strTempFile;
            delete szBuffer;
            szBuffer = NULL;
        }
        fileWrite.Close();
    }
    /****************************** 解密 ****************************/
    string strTemp;
    char ch;
    // 倒序
    // 单字节 + 0x50
    // 双字节 - 0x80
    strTemp = strTempFileData;
    strTempFileData = "";
    for (W_I16 i = strTemp.length() - 1; i >= 0; i--)
    {
        ch = strTemp.at(i);
        if (0 == (strTemp.length() - i - 1) % 2)
            ch -= 0x80;
        else
            ch += 0x50;
        strTempFileData += ch;
    }
    vector<vector<string>> AlldataSet;
    vector<string> vecStr;
    strTemp = "";
    // 取出数据
    for (W_I16 i = 0; i < strTempFileData.length(); i++)
    {
        ch = strTempFileData.at(i);
        if ('\t' == ch)
        {
            if (strTemp.length() > 0)
                vecStr.push_back(strTemp);
            strTemp = "";
        }
        else if ('\n' == ch)
        {
            if (strTemp.length() > 0)
            {
                vecStr.push_back(strTemp);
                strTemp = "";
                AlldataSet.push_back(vecStr);
                vecStr.clear();
            }
        }
        else if (i == strTempFileData.length() - 1 && strTemp.length() > 0)
        {
            vecStr.push_back(strTemp);
            strTemp = "";
            AlldataSet.push_back(vecStr);
            vecStr.clear();
        }
        else
        {
            strTemp += ch;
        }
    }
    for (W_I16 i = 0; i < AlldataSet.size(); i++)
    {
        if (AlldataSet[i][0] == List0)
        {
            iData = AlldataSet[i];
            break;
        }
    }

    return rect;
}

// 保存数据至文件Tempdata.dat
W_ErrorCode CSubaruBaseCanApp::InforSaveToFile(vector<string> iData)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CFile fileWrite;
    CString strFileNamePath = GetExePath();                    // 当前exe路径
    strFileNamePath += _T(Subaru_Special_DAT);    // 文件名

    string strTempFileData;
    /****************************** 读 ****************************/
    if (fileWrite.Open(strFileNamePath, CFile::modeRead))
    {
        int nBmpLeng = fileWrite.GetLength();
        if (nBmpLeng > 0)
        {
            char* szBuffer;
            szBuffer = new char[nBmpLeng + 1];
            memset(szBuffer, 0x00, nBmpLeng + 1);
            ASSERT(szBuffer);
            fileWrite.Read(szBuffer, nBmpLeng);
            string strTempFile(szBuffer);
            strTempFileData = strTempFile;
            delete szBuffer;
            szBuffer = NULL;
        }
        fileWrite.Close();
    }
    /****************************** 解密 ****************************/
    string strTemp;
    char ch;
    // 倒序
    // 单字节 + 0x50
    // 双字节 - 0x80
    strTemp = strTempFileData;
    strTempFileData = "";
    for (W_I16 i = strTemp.length() - 1; i >= 0; i--)
    {
        ch = strTemp.at(i);
        if (0 == (strTemp.length() - i - 1) % 2)
            ch -= 0x80;
        else
            ch += 0x50;
        strTempFileData += ch;
    }
    vector<vector<string>> AlldataSet;
    vector<string> vecStr;
    strTemp = "";
    // 取出数据
    for (W_I16 i = 0; i < strTempFileData.length(); i++)
    {
        ch = strTempFileData.at(i);
        if ('\t' == ch)
        {
            if (strTemp.length() > 0)
                vecStr.push_back(strTemp);
            strTemp = "";
        }
        else if ('\n' == ch)
        {
            if (strTemp.length() > 0)
            {
                vecStr.push_back(strTemp);
                strTemp = "";
                AlldataSet.push_back(vecStr);
                vecStr.clear();
            }
        }
        else if (i == strTempFileData.length() - 1 && strTemp.length() > 0)
        {
            vecStr.push_back(strTemp);
            strTemp = "";
            AlldataSet.push_back(vecStr);
            vecStr.clear();
        }
        else
        {
            strTemp += ch;
        }
    }
    /***************************** 更改 ****************************/
    for (W_I16 i = 0; i < AlldataSet.size(); i++)
    {
        if (AlldataSet[i][0] == iData[0])
        {
            AlldataSet[i] = iData;
            break;
        }
        else if (i >= AlldataSet.size() - 1)
        {
            AlldataSet.push_back(iData);
        }
    }
    if (0 == AlldataSet.size())
        AlldataSet.push_back(iData);
    /***************************** 加密 ***************************/
    strTemp = "";
    for (W_I16 i = 0; i < AlldataSet.size(); i++)
    {
        for (W_I16 j = 0; j < AlldataSet[i].size(); j++)
        {
            strTemp += AlldataSet[i][j];
            if (j == AlldataSet[i].size() - 1)
                strTemp += '\n';
            else
                strTemp += '\t';
        }
    }
    // 倒序
    // 单字节 - 0x50
    // 双字节 + 0x80
    strTempFileData = "";
    for (W_I16 i = strTemp.length() - 1; i >= 0; i--)
    {
        ch = strTemp.at(i);
        if (i % 2 == 0)
            ch += 0x80;
        else
            ch -= 0x50;
        strTempFileData += ch;
    }
    /****************************** 写入 ****************************/
    if (fileWrite.Open(strFileNamePath, CFile::modeCreate | CFile::modeWrite))
    {
        fileWrite.Write(strTempFileData.c_str(), strTempFileData.length());
        fileWrite.Close();
    }

    return rect;
}

// 读取DPF／机油相关的训练值。ECM -> TOOL
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_028c4f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200000004A        "是否开始读取柴油颗粒滤清器（DPF）/发动机机油相关的学习值？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x4A", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;

    vector<string> DsCmdSet;
    DsCmdSet.push_back("1186");
    DsCmdSet.push_back("1187");
    DsCmdSet.push_back("1188");
    DsCmdSet.push_back("1189");
    DsCmdSet.push_back("1156");
    DsCmdSet.push_back("1157");
    DsCmdSet.push_back("11A1");
    DsCmdSet.push_back("11A3");
    DsCmdSet.push_back("11A4");
    DsCmdSet.push_back("11A5");

    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = "22";
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl    ActTest;
    CBinary            binRecv;
    char            Temp[10];
    string            strValue = "";
    ActTest.InitCtrl(binMenu);
    ActTest.SetFirstRowFixed(false);
    ActTest.EnableAutoUnit(false);
    ActTest.SetColumnWid(50, 25, 25);
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAA", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAB", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAC", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAD", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAE", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAF", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB0", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB1", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB2", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB3", 6)));
    ActTest.SetItemUnit(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(1, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(2, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(3, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF2", 6)));
    ActTest.SetItemUnit(4, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(5, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF4", 6)));
    ActTest.SetItemUnit(6, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF4", 6)));
    ActTest.SetItemUnit(7, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(8, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(9, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF5", 6)));
    ActTest.AddOneBtn(Subaru_TTL_SAVE);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, binRecv)))
        return rect;
    if (binRecv.GetByteCount() < 7)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }
    else if (binRecv.GetByteCount() < 37)
    {
        for (int i = binRecv.GetByteCount(); i < 37; i++)
            binRecv += CBinary("\x00", 1);
    }

    sprintf(Temp, "%.1f", binRecv[3] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(0, strValue);
    sprintf(Temp, "%.1f", binRecv[6] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(1, strValue);
    sprintf(Temp, "%.1f", binRecv[9] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(2, strValue);
    sprintf(Temp, "%.3f", binRecv[12] * 0.005);
    strValue = Temp;
    ActTest.SetItemValue(3, strValue);
    sprintf(Temp, "%d", binRecv[15] * 256 + binRecv[16]);
    strValue = Temp;
    ActTest.SetItemValue(4, strValue);
    sprintf(Temp, "%d", binRecv[19] * 256 + binRecv[20]);
    strValue = Temp;
    ActTest.SetItemValue(5, strValue);
    sprintf(Temp, "%d", binRecv[23] * 256 + binRecv[24]);
    strValue = Temp;
    ActTest.SetItemValue(6, strValue);
    sprintf(Temp, "%d", binRecv[27] * 256 + binRecv[28]);
    strValue = Temp;
    ActTest.SetItemValue(7, strValue);
    sprintf(Temp, "%d", (binRecv[31] * 256 + binRecv[32]) * 10);
    strValue = Temp;
    ActTest.SetItemValue(8, strValue);
    sprintf(Temp, "%d", (binRecv[35] * 256 + binRecv[36]) * 5 + binRecv[35] / 8);
    strValue = Temp;
    ActTest.SetItemValue(9, strValue);

    W_I16 iRet = 3;
    while (-1 != iRet && 0 != iRet)
        iRet = ActTest.ShowCtrl();
    if (-1 == iRet)
        return rect;

    vector<string> iSaveData;
    iSaveData.push_back("dpf");
    string        iValue = "";

    sprintf(Temp, "%02X", binRecv[3]);
    iValue += Temp;
    sprintf(Temp, "%02X", binRecv[6]);
    iValue += Temp;
    sprintf(Temp, "%02X", binRecv[9]);
    iValue += Temp;
    sprintf(Temp, "%02X", binRecv[12]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[15], binRecv[16]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[19], binRecv[20]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[23], binRecv[24]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[27], binRecv[28]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[31], binRecv[32]);
    iValue += Temp;
    sprintf(Temp, "%02X%02X", binRecv[35], binRecv[36]);
    iValue += Temp;

    iSaveData.push_back(iValue);
    InforSaveToFile(iSaveData);
    // 0x5320000000B4        "关闭点火开关。\n取出电池，更换ECM。\n更换ECM后注册学习读值"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xCC", 6), DF_MB_OK, DT_CENTER);
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xB4", 6), DF_MB_OK, DT_CENTER);

    return rect;
}

// 写入DPF／机油相关的训练值。TOOL -> ECM
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_028b4f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53,0x00,0x00,0x01,0x01,0x87        "是否开始写入DPF/发动机机油相关学习值?"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x87", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    // 0x5320000000B5        "确认注解列中输入的信息指向目标车辆"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xB5", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;

    vector<string> DsCmdSet;
    DsCmdSet.push_back("2E1186");
    DsCmdSet.push_back("2E1187");
    DsCmdSet.push_back("2E1188");
    DsCmdSet.push_back("2E1189");
    DsCmdSet.push_back("2E1156");
    DsCmdSet.push_back("2E1157");
    DsCmdSet.push_back("2E11A1");
    DsCmdSet.push_back("2E11A3");
    DsCmdSet.push_back("2E11A4");
    DsCmdSet.push_back("2E11A5");

    CActTestCtrl    ActTest;
    char            Temp[10];
    string            strValue = "";
    ActTest.InitCtrl(binMenu);
    ActTest.SetFirstRowFixed(false);
    ActTest.EnableAutoUnit(false);
    ActTest.SetColumnWid(50, 25, 25);
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAA", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAB", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAC", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAD", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAE", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xAF", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB0", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB1", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB2", 6)));
    ActTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xB3", 6)));
    ActTest.SetItemUnit(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(1, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(2, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF1", 6)));
    ActTest.SetItemUnit(3, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF2", 6)));
    ActTest.SetItemUnit(4, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(5, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF4", 6)));
    ActTest.SetItemUnit(6, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF4", 6)));
    ActTest.SetItemUnit(7, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(8, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF3", 6)));
    ActTest.SetItemUnit(9, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF5", 6)));
    ActTest.AddOneBtn(Subaru_Btn_Write);

    vector<string> iData;
    rect = InforReadfromFile(iData, "dpf");
    if (iData.size() < 2)
    {
        // FF0200000001        "无可行数据"
        FxShowMessageBox(binMenu, CBinary("\xFF\x02\x00\x00\x00\x01", 6), DF_MB_OK, DT_CENTER);
        return rect;
    }
    CBinary binRecv = String2Binary(iData[1]);

    sprintf(Temp, "%.1f", binRecv[0] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(0, strValue);
    sprintf(Temp, "%.1f", binRecv[1] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(1, strValue);
    sprintf(Temp, "%.1f", binRecv[2] * 0.1);
    strValue = Temp;
    ActTest.SetItemValue(2, strValue);
    sprintf(Temp, "%.3f", binRecv[3] * 0.005);
    strValue = Temp;
    ActTest.SetItemValue(3, strValue);
    sprintf(Temp, "%d", binRecv[4] * 256 + binRecv[5]);
    strValue = Temp;
    ActTest.SetItemValue(4, strValue);
    sprintf(Temp, "%d", binRecv[6] * 256 + binRecv[7]);
    strValue = Temp;
    ActTest.SetItemValue(5, strValue);
    sprintf(Temp, "%d", binRecv[8] * 256 + binRecv[9]);
    strValue = Temp;
    ActTest.SetItemValue(6, strValue);
    sprintf(Temp, "%d", binRecv[10] * 256 + binRecv[11]);
    strValue = Temp;
    ActTest.SetItemValue(7, strValue);
    sprintf(Temp, "%d", (binRecv[12] * 256 + binRecv[13]) * 10);
    strValue = Temp;
    ActTest.SetItemValue(8, strValue);
    sprintf(Temp, "%d", (binRecv[14] * 256 + binRecv[15]) * 5 + binRecv[14] / 8);
    strValue = Temp;
    ActTest.SetItemValue(9, strValue);

    sprintf(Temp, "%02X", binRecv[0]);
    DsCmdSet[0] += Temp;
    sprintf(Temp, "%02X", binRecv[1]);
    DsCmdSet[1] += Temp;
    sprintf(Temp, "%02X", binRecv[2]);
    DsCmdSet[2] += Temp;
    sprintf(Temp, "%02X", binRecv[3]);
    DsCmdSet[3] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[4], binRecv[5]);
    DsCmdSet[4] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[6], binRecv[7]);
    DsCmdSet[5] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[8], binRecv[9]);
    DsCmdSet[6] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[10], binRecv[11]);
    DsCmdSet[7] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[12], binRecv[13]);
    DsCmdSet[8] += Temp;
    sprintf(Temp, "%02X%02X", binRecv[14], binRecv[15]);
    DsCmdSet[9] += Temp;

    W_I16 iRet = 3;
    while (-1 != iRet && 0 != iRet)
        iRet = ActTest.ShowCtrl();

    if (-1 == iRet)
        return rect;

    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
    {
        CBinary binCmd = String2Binary(DsCmdSet[i]);
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binCmd, binRecv)))
            return rect;
    }
    // 0x5320000000B6        "写入已完成"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xB6", 6), DF_MB_OK, DT_CENTER);
    m_pNetLayer->SendReceive(CBinary("\x10\x01", 2));

    return rect;
}

// 登入VIN
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02071120(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    string        strTemp, strShow;
    CBinary        binAns, binTemp;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(SubaruBRZ_SET->EnterCMD, binAns)))
        return rect;
    if (9 > binAns.GetByteCount())
    {
        FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x01\x00", 2), binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x09\x00", 2), binAns)))
        return rect;
    if (binAns.GetByteCount() < 6 || 0x40 > binAns[2])
    {
        FxShowMessageBox(binMenu, Subaru_TXT_MSG_SYSTEMNOTSUPPORTFUNCTION, DF_MB_OK, DT_CENTER);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x09\x02", 2), binAns)))
        return rect;
    string VinCode = FxGetStdString(CBinary("\x53\x4D\x9C\x3A\x16\x20", 6)) + ":";
    for (W_I16 i = 3; i < binAns.GetByteCount(); i++)
    {
        char Temp[4];
        sprintf(Temp, "%c", binAns[i]);
        VinCode += Temp;
    }
    if (DF_IDCANCEL == FxShowMessageBox(FxGetStdString(binMenu), VinCode, DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    CInputCtrl uiInput;
    CBinary bin2EF190("\x2e\xf1\x90\x56\x49\x4E\x3A\x20", 8);
    uiInput.InitCtrl(FxGetStdString(binMenu));
    W_I16 iRet = 0;
    while (1)
    {
        iRet = uiInput.ShowCtrl(binMenu, CBinary("\x53\x20\x00\x00\x00\xA7", 6), "AAAAAAAAAAAAAAAAA");
        if (DF_IDCANCEL == iRet || -1 == iRet)
            return rect;
        strTemp = uiInput.GetStringValue();
        if (strTemp.length() < 17)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xA5", 6), DF_MB_OK, DT_LEFT);
        }
        else
        {
            string ErrorChar = ":";
            binTemp.Clear();
            for (W_I16 i = 0; i < strTemp.length(); i++)
            {
                if ('I' == strTemp.at(i) || 'O' == strTemp.at(i))
                {
                    ErrorChar += strTemp.at(i);
                    break;
                }
                else
                {
                    binTemp += strTemp.at(i);
                }
            }
            if (ErrorChar.length() > 1)
                FxShowMessageBox(FxGetStdString(binMenu), FxGetStdString(CBinary("\x53\x20\x00\x00\x00\x05", 6)) + ErrorChar, DF_MB_OK, DT_LEFT);
            else
            {
                // 0x5320000000A2        "输入到ECU的VIN码是is "
                // 0x5320000000A3        "\n是否与车上显示的VIN牌相匹配"
                strShow = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xA2", 6)) + strTemp + FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xA3", 6));
                if (FxShowMessageBox(FxGetStdString(binMenu), strShow, DF_MB_YESNO, DT_CENTER) == DF_IDNO)
                    continue;
                break;
            }
        }
    }

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    binAns = SubaruCanSendReceive(bin2EF190 + binTemp);
    if (0 >= binAns.GetByteCount())
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    else if (0x7f == binAns[0])
    {
        FxShowMessageBox(STD_TTL_MSG_ERASEDTC, Subaru_TXT_MSG_SYSTEMNOTSUPPORTFUNCTION, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x10\x01", 2), binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x01\x00", 2), binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x09\x00", 2), binAns)))
        return rect;
    if (binAns.GetByteCount() < 6 || 0x40 > binAns[2])
    {
        FxShowMessageBox(binMenu, Subaru_TXT_MSG_SYSTEMNOTSUPPORTFUNCTION, DF_MB_OK, DT_CENTER);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(CBinary("\x09\x02", 2), binAns)))
        return rect;
    VinCode = "";
    for (W_I16 i = 3; i < binAns.GetByteCount(); i++)
    {
        char Temp[4];
        sprintf(Temp, "%c", binAns[i]);
        VinCode += Temp;
    }
    if (VinCode == strTemp)
    {
        // 0x5320000000A9        "成功输入VIN码"        
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xA9", 6), DF_MB_OK, DT_CENTER);
    }
    else
    {
        // 0x5320000000A0        "写入失败"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xA0", 6), DF_MB_OK, DT_CENTER);
    }

    return rect;
}

// 新注册喷油器代码（TOOL至ECM）
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02674f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CMenuCtrl uiMenu;

    W_I16 iSelect = 0;
    vector<string> vctStrPrompt;
    vector<string> vctStrMask;
    vector<string> vctStrDefault;
    for (W_I16 i = 0; i < 8; i++)
    {
        char buf[2];
        vctStrPrompt.push_back(itoa(i, buf, 10));
        vctStrDefault.push_back("");
        if (i < 7)
            vctStrMask.push_back("HHHH");
        else
            vctStrMask.push_back("HH");
    }

    W_I16 iRet = 0;
    CInputCtrl uiInput;
    CBinary binTemp;
    vector<string> vecStrInput;
    string strTemp;
    while (1)
    {
        uiMenu.InitCtrl(binMenu);
        // 0x532000000053        "#1喷油器"
        // 0x532000000054        "#2喷油器"
        // 0x532000000055        "#3喷油器"
        // 0x532000000056        "#4喷油器"
        uiMenu.AddOneItem(CBinary("\x53\x20\x00\x00\x00\x53", 6));
        uiMenu.AddOneItem(CBinary("\x53\x20\x00\x00\x00\x54", 6));
        uiMenu.AddOneItem(CBinary("\x53\x20\x00\x00\x00\x55", 6));
        uiMenu.AddOneItem(CBinary("\x53\x20\x00\x00\x00\x56", 6));
        iSelect = uiMenu.ShowCtrl();
        if (-1 == iSelect)
            break;
        binTemp = CBinary("\x53\x20\x00\x00\x00\x53", 6);
        binTemp.SetByteAt(5, binTemp[5] + iSelect);
        while (1)
        {
            iRet = uiInput.ShowMultiInput(FxGetStdString(binTemp), vctStrPrompt, vctStrMask, vctStrDefault);
            if (-1 == iRet || DF_IDCANCEL == iRet)
                break;
            vecStrInput = uiInput.GetMultiInputVctStr();

            strTemp = FxGetStdString(binTemp) + "\n";
            string strInput = "";
            for (W_I16 i = 0; i < vecStrInput.size(); i++)
            {
                strTemp += vecStrInput[i];
                strInput += vecStrInput[i];
                if (i != vecStrInput.size() - 1)
                    strTemp += 0x20;
            }
            // 0x5320000000FF        "\n是否注册？"
            strTemp += FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xFF", 6));
            if (DF_IDCANCEL != FxShowMessageBox(FxGetStdString(binTemp), strTemp, DF_MB_OKCANCEL, DT_LEFT))
            {
                CBinary binAns;
                if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
                    break;
                CBinary binCmd("\x2E\x10\x2A", 3);
                binCmd.SetByteAt(2, binCmd[2] + iSelect);
                if (strInput.length() % 2 != 0)
                {
                    FxShowMessageBox(binMenu, Subaru_TXT_MSG_SYSTEMNOTSUPPORTFUNCTION, DF_MB_OK, DT_LEFT);
                    break;
                }
                binCmd += String2Binary(strInput);
                if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binCmd, binAns)))
                    break;
                // 0x5320000000CB        "已完成注册。\n是否继续注册？"
                if (DF_IDCANCEL == FxShowMessageBox(FxGetStdString(binTemp), FxGetStdString(binTemp) + "\n"
                    + FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCB", 6)), DF_MB_OKCANCEL, DT_LEFT))
                    return rect;
                break;
            }
        }
    }

    return rect;
}

// 读取喷油器代码（ECM至TOOL）
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02664f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200000005A        "是否开始读取喷油器代码？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x5A", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;

    string strTemp, strDisPlay;
    CBinary bin22102A("\x22\x10\x2A", 3);
    CBinary binTemp, binAns;
    vector<string> iData;
    iData.push_back("inj");
    for (W_I16 i = 0; i < 4; i++)
    {
        binTemp = bin22102A;
        binTemp.SetByteAt(2, binTemp[2] + i);
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binTemp, binAns)))
            return rect;
        if (binAns.GetByteCount() < 18)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_MSG_SYSTEMNOTSUPPORTFUNCTION, DF_MB_OK, DT_CENTER);
            return rect;
        }
        // 0x532000000053        "#1喷油器"
        // 0x532000000054        "#2喷油器"
        // 0x532000000055        "#3喷油器"
        // 0x532000000056        "#4喷油器"
        binTemp = CBinary("\x53\x20\x00\x00\x00\x53", 6);
        binTemp.SetByteAt(5, binTemp[5] + i);
        strDisPlay += FxGetStdString(binTemp);
        strDisPlay += ":";
        char buf[3];
        strTemp = "";
        for (W_I16 j = 3; j < binAns.GetByteCount(); j++)
        {
            strTemp += itoa(binAns[j], buf, 16);
            strDisPlay += itoa(binAns[j], buf, 16);
            if (0 == j % 2)
                strDisPlay += 0x20;
        }
        strDisPlay += "\n";
        iData.push_back(strTemp);
    }
    // 0x5320000000C7        "是否保存数据？"
    strDisPlay += FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xC7", 6));
    if (DF_IDCANCEL == FxShowMessageBox(FxGetStdString(binMenu), strDisPlay, DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    // 0x5320000000C9        "关闭点火开关。\n拆卸电池，更换ECM。\n更换后注册读取代码。"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC9", 6), DF_MB_OK, DT_CENTER);
    InforSaveToFile(iData);

    return rect;
}

// 注册正在读取的代码（TOOL至ECM）
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02654f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    string strDisPlay;
    CBinary bin2E102A("\x2E\x10\x2A", 3);
    CBinary binTemp, binAns;
    CBinaryGroup bgCmd;
    vector<string> iData;
    InforReadfromFile(iData, "inj");
    if (iData.size() < 2)
    {
        // FF0200000001        "无可行数据"
        FxShowMessageBox(binMenu, CBinary("\xFF\x02\x00\x00\x00\x01", 6), DF_MB_OK, DT_CENTER);
        return rect;
    }
    for (W_I16 i = 0; i < 4; i++)
    {
        binTemp = bin2E102A;
        binTemp.SetByteAt(2, binTemp[2] + i);
        binTemp += String2Binary(iData[i + 1]);
        bgCmd.Append(binTemp);
        // 0x532000000053        "#1喷油器"
        // 0x532000000054        "#2喷油器"
        // 0x532000000055        "#3喷油器"
        // 0x532000000056        "#4喷油器"
        binTemp = CBinary("\x53\x20\x00\x00\x00\x53", 6);
        binTemp.SetByteAt(5, binTemp[5] + i);
        strDisPlay += FxGetStdString(binTemp);
        strDisPlay += ":";
        char buf[3];
        for (W_I16 j = 3; j < bgCmd[i].GetByteCount(); j++)
        {
            strDisPlay += itoa(bgCmd[i][j], buf, 16);
            if (0 == j % 2)
                strDisPlay += 0x20;
        }
        strDisPlay += "\n";
    }
    // 0x5320000000FF        "\n是否注册？"
    strDisPlay += FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xFF", 6));
    if (DF_IDCANCEL == FxShowMessageBox(FxGetStdString(binMenu), strDisPlay, DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    for (W_I16 i = 0; i < bgCmd.GetByteCount(); i++)
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bgCmd[i], binAns)))
            return rect;
    }
    // 0x53200000005B        "已完成注册"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x5B", 6), DF_MB_OK, DT_CENTER);

    return rect;
}

// 喷油器代码显示
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02644f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200000005A        "是否开始读取喷油器代码？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x5A", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;

    rect = ProtectCal();
    if (rect != CErrorCode::EC_SUCCESS)
        return rect;

    CBinary bin22102A("\x22\x10\x2A", 3);
    CBinary binTemp, binAns;
    vector<string> injectID;
    string strTemp;

    for (W_I16 i = 0; i < 4; i++)
    {
        binTemp = bin22102A;
        binTemp.SetByteAt(2, binTemp[2] + i);
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binTemp, binAns)))
            return rect;
        if (binAns.GetByteCount() < 18)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_MSG_SYSTEMNOTSUPPORT, DF_MB_OK, DT_LEFT);
            return rect;
        }
        for (W_I16 j = 0; j < 3; j++)
            binAns.DeleteByte(0);
        strTemp = BinaryToString_MO(binAns);
        string strTempTemp = "";
        for (W_I16 j = 0; j < strTemp.length(); j++)
        {
            strTempTemp += strTemp.at(j);
            if (0 == (j + 1) % 4)
                strTempTemp += 0x20;
        }
        injectID.push_back(strTempTemp);
    }
    strTemp = "";
    for (W_I16 i = 0; i < injectID.size(); i++)
    {
        binTemp = Subaru_TXT_MSG_InjectID1;
        binTemp.SetByteAt(5, binTemp[5] + i);
        strTemp = strTemp + FxGetStdString(binTemp) + ":" + injectID[i] + "\n";
    }
    FxShowMessageBox(FxGetStdString(binMenu), strTemp, DF_MB_OK, DT_LEFT);

    return rect;
}

// 燃油泵继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11de == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x01, vctStr, binMenu);

    return rect;
}

// CPC 电磁阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_03871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x10ae == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x02, vctStr, binMenu);

    return rect;
}

// 散热器风扇继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_00871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11dc == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x03, vctStr, binMenu);

    return rect;
}

// 空调压缩机继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_01871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11da == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x04, vctStr, binMenu);

    return rect;
}

// PCV 电磁阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_04871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11e2 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x07, vctStr, binMenu);

    return rect;
}

// 排气控制电磁阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_05871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11e5 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x08, vctStr, binMenu);

    return rect;
}

// 油箱内力交换电磁阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_09871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11e6 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x0C, vctStr, binMenu);

    return rect;
}

// 涡轮增压器废气口电磁线圈
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_0e871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x10ac == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x1D, vctStr, binMenu);

    return rect;
}

// 辅助空气组合阀 1
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_0c871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1248 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x1f, vctStr, binMenu);

    return rect;
}

// 辅助空气组合阀 2
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_0d871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1246 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x10, vctStr, binMenu);

    return rect;
}

// 辅助气泵继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_12871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1247 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x11, vctStr, binMenu);

    return rect;
}

// CPC电磁阀2
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_13871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x11f0 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x12, vctStr, binMenu);

    return rect;
}

// 辅助燃油泵继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_10871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1251 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x13, vctStr, binMenu);

    return rect;
}

// 燃油压切换电磁阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_11871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x124b == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x14, vctStr, binMenu);

    return rect;
}

// ELCM开关阀
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_16871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1272 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x20, vctStr, binMenu);

    return rect;
}

// ELCM泵
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_17871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back(SystemNum);
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1273 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x21, vctStr, binMenu);

    return rect;
}

// 刹车真空泵继电器
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_14871c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    CSearchString SubaData;
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vctStr;
    vector<vector<string>> SearchResault;
    vctStr.push_back("0000006B");
    SubaData.SearchString(SearchResault, FALSE, 0, 0, vctStr);
    for (W_I16 i = 0; i < SearchResault.size(); i++)
    {
        vctStr = SearchResault[i];
        if (0x1283 == StringToHex(vctStr[2]))
            break;
        vctStr.clear();
    }
    if (vctStr.size() <= 0)
        return CErrorCode::EC_DATA;

    rect = ActuatorONOFFOperation(0x22, vctStr, binMenu);

    return rect;
}

// 致动器开关操作->所有功能主要过程
W_ErrorCode CSubaruBaseCanApp::ActuatorONOFFOperation(W_U8 cmd, vector<string> dsData, CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    rect = ProtectCal();
    if (rect != CErrorCode::EC_SUCCESS)
        return rect;
    CBinary bin310140("\x31\x01\x40", 3);
    CBinary bin310340("\x31\x03\x40", 3);
    CBinary bin310240("\x31\x02\x40", 3);
    bin310140 += cmd;
    bin310340 += cmd;
    bin310240 += cmd;

    CBinary binAns1, binAns2;
    string strTemp;
    CActTestCtrl ActTest;
    ActTest.InitCtrl(binMenu);
    ActTest.SetFirstRowFixed(false);
    /*if (atoi(dsData[8].c_str()) > 0)
    {
    strTemp = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(dsData[8]));
    ActTest.AddOneItem(FxGetStdString(binMenu), strTemp);
    ActTest.SetColumnWid(38, 38, 24);
    }
    else*/
    //{
    ActTest.AddOneItem(FxGetStdString(binMenu));
    ActTest.SetColumnWid(70, 30, 0);
    //}

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin310140, binAns1)))
        return rect;

    W_I16 iRet = 0;
    DWORD iTimer = 0;
    CBinary binCmd = String2Binary(dsData[1] + dsData[2]);
    while (1)
    {
        if (GetTickCount() > iTimer)
        {
            // 间隔150毫秒发一次命令
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin310340, binAns1)))
                return rect;
        }
        if (binAns1.GetByteCount() > 5 && 0x00 == binAns1[4] && 0x01 == binAns1[5])
        {
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binCmd, binAns2)))
                return rect;
            for (W_I16 j = 0; j < binCmd.GetByteCount() && binAns2.GetByteCount() > 0; j++)
                binAns2.DeleteByte(0);

            if (cmd == 0x1D)
            {
                char Temp[10];
                sprintf(Temp, "%.1f", (float)100.0 / 255 * binAns2[0]);
                strTemp = Temp;
            }
            else
            {
                // 0x53,0x20,0x00,0x00,0x01,0x01                "OFF"
                // 0x53,0x20,0x00,0x00,0x00,0xFE                "ON"
                if (binAns2.GetByteCount() > 0 && 0x00 == binAns2[0])
                    strTemp = FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x01", 6));
                else if (binAns2.GetByteCount() > 0 && 0xff == binAns2[0])
                    strTemp = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xFE", 6));
                else
                    strTemp = FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x02", 6));//0x53,0x20,0x00,0x00,0x01,0x02                "---"
            }

            ActTest.SetItemValue(0, strTemp);
            iRet = ActTest.ShowCtrl();
            if (-1 == iRet)
                break;
        }
        else
        {
            iRet = FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
            if (DF_IDCANCEL == iRet)
                return rect;
        }
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin310240, binAns1)))
        return rect;
    while (1)
    {
        if (GetTickCount() > iTimer)
        {
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin310340, binAns1)))
                return rect;
            if (binAns1.GetByteCount() <= 5 || 0x00 != binAns1[4] || 0x01 != binAns1[5])
                break;
        }
        iRet = FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
        if (DF_IDCANCEL == iRet)
            return rect;
    }

    return rect;
}

// 燃油泵能率记忆
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02774f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200000008B        "是否开始学习？\n如果学习过程中途中断，必须重新开始，务必完成学习。"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8B", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    // 0x53200000008C        "必需工作条件均已满足之后才能开始学习。（详情请参阅《维修手册》）"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8C", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    // 0x53200000008D        "启动发动机，保持发动机以2500转/分以上的转速运行5秒以上。"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8D", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    // 0x53200000008E        "发动机转速回位至怠速状态。开始学习。"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8E", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;

    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;
    CBinary bin31017000("\x31\x01\x70\x00", 4);
    CBinary bin31037000("\x31\x03\x70\x00", 4);
    CBinary bin31027000("\x31\x02\x70\x00", 4);
    CBinary ExitCmd("\x10\x01", 2);
    CBinary binTemp, binAns;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017000, binAns)))
        return rect;

    CSearchString SubaData;
    vector<string> DsCmdSet;
    DsCmdSet.push_back("1253");
    DsCmdSet.push_back("1172");
    DsCmdSet.push_back("F405");
    DsCmdSet.push_back("F40C");
    DsCmdSet.push_back("F40D");
    DsCmdSet.push_back("F442");
    DsCmdSet.push_back("10A6");
    DsCmdSet.push_back("10A7");
    DsCmdSet.push_back("F423");
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vecStr;
    vector<vector<string> > SearchReasault;
    vector<vector<string> > ReasaultTemp;
    // 查找并匹配库中存在的数据流项
    vecStr.push_back(SystemNum);
    SubaData.SearchString(SearchReasault, FALSE, 0, 0, vecStr);

    for (W_I16 j = 0; j < DsCmdSet.size(); j++)
    {
        for (W_I16 i = 0; i < SearchReasault.size(); i++)
        {
            if (StringToHex(DsCmdSet[j]) == StringToHex(SearchReasault[i][2]))
            {
                ReasaultTemp.push_back(SearchReasault[i]);
                break;
            }
        }
    }
    if (ReasaultTemp.size() <= 0)
        return CErrorCode::EC_DATA;
    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = ReasaultTemp[0][1];
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl ActTest;
    // 0x5320000000D2        "不要触碰车辆\n按【取消】停止"
    ActTest.InitCtrl(CBinary("\x53\x20\x00\x00\x00\xD2", 6));
    ActTest.SetFirstRowFixed(false);
    ActTest.SetColumnWid(50, 25, 25);
    for (W_I16 i = 0; i < 10 && i < (ReasaultTemp.size() + 1); i++)
    {
        // 0x5320000000D4        "燃油泵学习状态"
        binTemp = CBinary("\x53\x20\x00\x00\x00\xD4", 6);
        binTemp.SetByteAt(5, binTemp[5] + i);
        string head;
        if (i > 1 && 0x00 == StringToHex(ReasaultTemp[i - 1][3]))
            head = Subaru_SPECMSG;
        else
            head = Subaru_IDCAN;
        if (i > 1)
            ActTest.AddOneItem(FxGetStdString(binTemp), FxGetStdString(CBinary(head.c_str(), 2) + String2Binary(ReasaultTemp[i - 1][8])));
        else
            ActTest.AddOneItem(FxGetStdString(binTemp));
    }

    DWORD iTimer = 0;
    W_I16 iRet = 0;
    W_I16 iTemp;
    CBinary recv1;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            SendAndRecive(bin31037000, binAns);
            if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
                break;
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x02 == binAns[5]))
        {
            if (0x02 == binAns[5])
                ActTest.m_strTitle = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)).c_str();

            // 0x5320000000CE        "未学习"
            // 0x5320000000CF        "已结束学习"
            if (0x01 == binAns[5])
                ActTest.SetItemValue(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCE", 6)));
            else
                ActTest.SetItemValue(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)));
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, recv1)))
                return rect;

            for (W_I16 i = 0; i < ReasaultTemp.size(); i++)
            {
                binTemp.Clear();
                W_I16 StartPos = 1;
                for (W_I16 j = 0; j < i; j++)
                {
                    StartPos += String2Binary(ReasaultTemp[j][2]).GetByteCount();
                    StartPos += StringToHex(ReasaultTemp[j][6]);
                }
                StartPos += String2Binary(ReasaultTemp[i][2]).GetByteCount();
                iTemp = StringToHex(ReasaultTemp[i][6]);
                for (W_I16 j = 0; j < iTemp && StartPos < recv1.GetByteCount(); j++)
                    binTemp += recv1[StartPos++];
                strTemp = SubaDataCanCal(binTemp, ReasaultTemp[i]);
                if (strTemp.length() <= 0)
                    strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                ActTest.SetItemValue(1 + i, strTemp);
            }
            do { iRet = ActTest.ShowCtrl(); } while (0x02 == binAns[5] && -1 != iRet);
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
        if (-1 == iRet)
            break;
    }
    if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x10 == binAns[5]))
    {
        // 0x5320000000CD        "未能完成学习"
        // 0x5320000000D3        "过程已中止"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xCD", 6), DF_MB_OK, DT_CENTER);
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xD3", 6), DF_MB_OK, DT_CENTER);
    }
    else if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x02 == binAns[5])
    {
        bin31027000.SetByteAt(3, 0x01);
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31027000, binAns)))
        return rect;
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 燃油喷射器喷射量记忆
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02764f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;


    // 0x5320000000DF        "喷油器是否已更换？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xDF", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;

    CBinary binAns, binTemp;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31017002("\x31\x01\x70\x02", 4);
    CBinary bin31037002("\x31\x03\x70\x02", 4);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017002, binAns)))
        return rect;

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31037002, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x02 == binAns[5])
            break;
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
        {
            // 0x532000000080        "执行条件不成立"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x80", 6), DF_MB_OK, DT_CENTER);
            return CErrorCode::EC_SUCCESS;
        }
        if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
            return rect;
    }

    // 0x532000000093        "注意学习过程中发动机转速的变化"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x93", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;

    CBinary bin31017001("\x31\x01\x70\x01", 4);
    CBinary bin31037001("\x31\x03\x70\x01", 4);
    CBinary bin31027001("\x31\x02\x70\x01", 4);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017001, binAns)))
        return rect;

    CSearchString SubaData;
    vector<string> DsCmdSet;
    DsCmdSet.push_back("1254");
    DsCmdSet.push_back("F405");
    DsCmdSet.push_back("F40C");
    DsCmdSet.push_back("F40D");
    DsCmdSet.push_back("F442");
    DsCmdSet.push_back("F433");
    DsCmdSet.push_back("10A6");
    DsCmdSet.push_back("10A7");
    DsCmdSet.push_back("F423");
    DsCmdSet.push_back("111F");
    DsCmdSet.push_back("112A");
    DsCmdSet.push_back("112B");
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vecStr;
    vector<vector<string> > SearchReasault;
    vector<vector<string> > ReasaultTemp;
    // 查找并匹配库中存在的数据流项
    vecStr.push_back(SystemNum);
    SubaData.SearchString(SearchReasault, FALSE, 0, 0, vecStr);

    for (W_I16 j = 0; j < DsCmdSet.size(); j++)
    {
        for (W_I16 i = 0; i < SearchReasault.size(); i++)
        {
            if (StringToHex(DsCmdSet[j]) == StringToHex(SearchReasault[i][2]))
            {
                ReasaultTemp.push_back(SearchReasault[i]);
                break;
            }
        }
    }
    if (ReasaultTemp.size() <= 0)
        return CErrorCode::EC_DATA;
    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = ReasaultTemp[0][1];
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl ActTest;
    // 0x5320000000D2        "不要触碰车辆\n按【取消】停止"
    ActTest.InitCtrl(CBinary("\x53\x20\x00\x00\x00\xD2", 6));
    ActTest.SetFirstRowFixed(false);
    ActTest.SetColumnWid(50, 25, 25);
    for (W_I16 i = 0; i < 13 && i < (ReasaultTemp.size() + 1); i++)
    {
        if (i > 0)
        {
            binTemp = CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i - 1][7]);
            if (0x00 != StringToHex(ReasaultTemp[i - 1][8]))
                ActTest.AddOneItem(FxGetStdString(binTemp), FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i - 1][8])));
            else
                ActTest.AddOneItem(FxGetStdString(binTemp));
        }
        else
        {
            // 0x5320000000e0        "喷油器燃油喷射量学习状态"
            binTemp = CBinary("\x53\x20\x00\x00\x00\xe0", 6);
            ActTest.AddOneItem(FxGetStdString(binTemp));
        }
    }

    iTimer = 0;
    W_I16 iRet = 0;
    W_I16 iTemp;
    CBinary recv1;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31037001, binAns)))
                return rect;
            if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
            {
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xE6", 6), DF_MB_OK, DT_LEFT);
                break;
            }
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x02 == binAns[5]))
        {
            if (0x02 == binAns[5])
                ActTest.m_strTitle = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)).c_str();

            // 0x5320000000CE        "未学习"
            // 0x5320000000CF        "已结束学习"
            if (0x01 == binAns[5])
                ActTest.SetItemValue(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCE", 6)));
            else
                ActTest.SetItemValue(0, FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)));
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, recv1)))
                return rect;

            for (W_I16 i = 0; i < ReasaultTemp.size(); i++)
            {
                binTemp.Clear();
                W_I16 StartPos = 1;
                for (W_I16 j = 0; j < i; j++)
                {
                    StartPos += String2Binary(ReasaultTemp[j][2]).GetByteCount();
                    StartPos += StringToHex(ReasaultTemp[j][6]);
                }
                StartPos += String2Binary(ReasaultTemp[i][2]).GetByteCount();
                iTemp = StringToHex(ReasaultTemp[i][6]);
                for (W_I16 j = 0; j < iTemp && StartPos < recv1.GetByteCount(); j++)
                    binTemp += recv1[StartPos++];
                strTemp = SubaDataCanCal(binTemp, ReasaultTemp[i]);
                if (strTemp.length() <= 0)
                    strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                ActTest.SetItemValue(1 + i, strTemp);
            }
            do { iRet = ActTest.ShowCtrl(); } while (0x02 == binAns[5] && -1 != iRet);
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
        if (-1 == iRet)
            break;
    }
    // 0x5320000000D3        "过程已中止"
    if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x10 == binAns[5]))
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xD3", 6), DF_MB_OK, DT_CENTER);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31027001, binAns)))
        return rect;
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// EGR阀门开角记忆
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02754f20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x532000000091        "是否开始学习？"
    // 0x532000000092        "冷却液温度保持20℃到60℃时是否能完成学习？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x91", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x92", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;

    CBinary binTemp, binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31017003("\x31\x01\x70\x03", 4);
    CBinary bin31037003("\x31\x03\x70\x03", 4);
    CBinary bin31027003("\x31\x02\x70\x03", 4);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017003, binAns)))
        return rect;

    CSearchString SubaData;
    vector<string> DsCmdSet;
    DsCmdSet.push_back("1257");
    DsCmdSet.push_back("F405");
    DsCmdSet.push_back("F442");
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vecStr;
    vector<vector<string> > SearchReasault;
    vector<vector<string> > ReasaultTemp;
    // 查找并匹配库中存在的数据流项
    vecStr.push_back(SystemNum);
    SubaData.SearchString(SearchReasault, FALSE, 0, 0, vecStr);

    for (W_I16 j = 0; j < DsCmdSet.size(); j++)
    {
        for (W_I16 i = 0; i < SearchReasault.size(); i++)
        {
            if (StringToHex(DsCmdSet[j]) == StringToHex(SearchReasault[i][2]))
            {
                ReasaultTemp.push_back(SearchReasault[i]);
                break;
            }
        }
    }
    if (ReasaultTemp.size() <= 0)
        return CErrorCode::EC_DATA;
    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = ReasaultTemp[0][1];
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl ActTest;
    // 0x5320000000EA        "正在学习中"
    ActTest.InitCtrl(CBinary("\x53\x20\x00\x00\x00\xEA", 6));
    ActTest.SetFirstRowFixed(false);
    ActTest.SetColumnWid(50, 25, 25);
    for (W_I16 i = 0; i < 3 && i < ReasaultTemp.size(); i++)
    {
        binTemp = CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][7]);
        if (0x00 != StringToHex(ReasaultTemp[i][8]))
            ActTest.AddOneItem(FxGetStdString(binTemp), FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][8])));
        else
            ActTest.AddOneItem(FxGetStdString(binTemp));
    }

    DWORD iTimer = 0;
    W_I16 iRet = 0;
    W_I16 iTemp;
    CBinary recv1;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31037003, binAns)))
                return rect;
            if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
            {
                // 0x532000000080        "执行条件不成立"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x80", 6), DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_SUCCESS;
            }
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x02 == binAns[5]))
        {
            // 0x5320000000E9        "关闭点火开关。开关点火开关后在'当前数据显示'中确认废气再循环（EGR）学习状态。"
            if (0x02 == binAns[5])
                ActTest.m_strTitle = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xE9", 6)).c_str();

            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, recv1)))
                return rect;
            for (W_I16 i = 0; i < ReasaultTemp.size(); i++)
            {
                binTemp.Clear();
                W_I16 StartPos = 1;
                for (W_I16 j = 0; j < i; j++)
                {
                    StartPos += String2Binary(ReasaultTemp[j][2]).GetByteCount();
                    StartPos += StringToHex(ReasaultTemp[j][6]);
                }
                StartPos += String2Binary(ReasaultTemp[i][2]).GetByteCount();
                iTemp = StringToHex(ReasaultTemp[i][6]);
                for (W_I16 j = 0; j < iTemp && StartPos < recv1.GetByteCount(); j++)
                    binTemp += recv1[StartPos++];
                strTemp = SubaDataCanCal(binTemp, ReasaultTemp[i]);
                if (strTemp.length() <= 0)
                    strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                ActTest.SetItemValue(i, strTemp);
            }
            do { iRet = ActTest.ShowCtrl(); } while (0x02 == binAns[5] && -1 != iRet);
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
        if (-1 == iRet)
            break;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31027003, binAns)))
        return rect;
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 涡轮叶片角记忆
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02374c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x532000000091        "是否开始学习？"
    // 0x53200000008C        "必需工作条件均已满足之后才能开始学习。（详情请参阅《维修手册》）"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x91", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8C", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;

    CBinary binTemp, binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31017004("\x31\x01\x70\x04", 4);
    CBinary bin31037004("\x31\x03\x70\x04", 4);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017004, binAns)))
        return rect;

    CSearchString SubaData;
    vector<string> DsCmdSet;
    DsCmdSet.push_back("125E");
    DsCmdSet.push_back("F405");
    DsCmdSet.push_back("F40C");
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vecStr;
    vector<vector<string> > SearchReasault;
    vector<vector<string> > ReasaultTemp;
    // 查找并匹配库中存在的数据流项
    vecStr.push_back(SystemNum);
    SubaData.SearchString(SearchReasault, FALSE, 0, 0, vecStr);

    for (W_I16 j = 0; j < DsCmdSet.size(); j++)
    {
        for (W_I16 i = 0; i < SearchReasault.size(); i++)
        {
            if (StringToHex(DsCmdSet[j]) == StringToHex(SearchReasault[i][2]))
            {
                ReasaultTemp.push_back(SearchReasault[i]);
                break;
            }
        }
    }
    if (ReasaultTemp.size() <= 0)
        return CErrorCode::EC_DATA;
    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = ReasaultTemp[0][1];
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl ActTest;
    // 0x5320000000D2        "不要触碰车辆\n按【取消】停止"
    ActTest.InitCtrl(CBinary("\x53\x20\x00\x00\x00\xD2", 6));
    ActTest.SetFirstRowFixed(false);
    ActTest.SetColumnWid(50, 25, 25);
    for (W_I16 i = 0; i < 3 && i < ReasaultTemp.size(); i++)
    {
        binTemp = CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][7]);
        if (0x00 != StringToHex(ReasaultTemp[i][8]))
            ActTest.AddOneItem(FxGetStdString(binTemp), FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][8])));
        else
            ActTest.AddOneItem(FxGetStdString(binTemp));
    }

    DWORD iTimer = 0;
    W_I16 iRet = 0;
    W_I16 iTemp;
    CBinary recv1;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31037004, binAns)))
                return rect;
            if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
            {
                // 0x532000000080        "执行条件不成立"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x80", 6), DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_SUCCESS;
            }
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x02 == binAns[5]))
        {
            // 0x5320000000CF        "已结束学习"
            if (0x02 == binAns[5])
                ActTest.m_strTitle = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)).c_str();

            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, recv1)))
                return rect;
            for (W_I16 i = 0; i < ReasaultTemp.size(); i++)
            {
                binTemp.Clear();
                W_I16 StartPos = 1;
                for (W_I16 j = 0; j < i; j++)
                {
                    StartPos += String2Binary(ReasaultTemp[j][2]).GetByteCount();
                    StartPos += StringToHex(ReasaultTemp[j][6]);
                }
                StartPos += String2Binary(ReasaultTemp[i][2]).GetByteCount();
                iTemp = StringToHex(ReasaultTemp[i][6]);
                for (W_I16 j = 0; j < iTemp && StartPos < recv1.GetByteCount(); j++)
                    binTemp += recv1[StartPos++];
                strTemp = SubaDataCanCal(binTemp, ReasaultTemp[i]);
                if (strTemp.length() <= 0)
                    strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                ActTest.SetItemValue(i, strTemp);
            }
            do { iRet = ActTest.ShowCtrl(); } while (0x02 == binAns[5] && -1 != iRet);
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
        if (-1 == iRet)
            break;
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 气流传感器记忆
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02274c20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x532000000091        "是否开始学习？"
    // 0x53200000008C        "必需工作条件均已满足之后才能开始学习。（详情请参阅《维修手册》）"
    // 0x532000000093        "注意学习过程中发动机转速的变化"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x91", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x8C", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x93", 6), DF_MB_OKCANCEL, DT_LEFT))
        return rect;
    rect = ProtectCal();
    if (CErrorCode::EC_SUCCESS != rect)
        return rect;

    CBinary binTemp, binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31017005("\x31\x01\x70\x05", 4);
    CBinary bin31037005("\x31\x03\x70\x05", 4);
    CBinary bin31027005("\x31\x02\x70\x05", 4);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31017005, binAns)))
        return rect;

    CSearchString SubaData;
    vector<string> DsCmdSet;
    DsCmdSet.push_back("1261");
    DsCmdSet.push_back("F405");
    DsCmdSet.push_back("F40C");
    DsCmdSet.push_back("F433");
    DsCmdSet.push_back("F446");
    if (FALSE == SubaData.OpenTabFile(Subaru_Data_CBF))
        return CErrorCode::EC_DATA;

    vector<string> vecStr;
    vector<vector<string> > SearchReasault;
    vector<vector<string> > ReasaultTemp;
    // 查找并匹配库中存在的数据流项
    vecStr.push_back(SystemNum);
    SubaData.SearchString(SearchReasault, FALSE, 0, 0, vecStr);

    for (W_I16 j = 0; j < DsCmdSet.size(); j++)
    {
        for (W_I16 i = 0; i < SearchReasault.size(); i++)
        {
            if (StringToHex(DsCmdSet[j]) == StringToHex(SearchReasault[i][2]))
            {
                ReasaultTemp.push_back(SearchReasault[i]);
                break;
            }
        }
    }
    if (ReasaultTemp.size() <= 0)
        return CErrorCode::EC_DATA;
    // 将所有命令以 22/21 CMD1 CMD2 CMD3...的格式保存
    string strTemp = ReasaultTemp[0][1];
    for (W_I16 i = 0; i < DsCmdSet.size(); i++)
        strTemp += DsCmdSet[i];
    CBinary binDsCmd = String2Binary(strTemp);

    CActTestCtrl ActTest;
    // 0x5320000000D2        "不要触碰车辆\n按【取消】停止"
    ActTest.InitCtrl(CBinary("\x53\x20\x00\x00\x00\xD2", 6));
    ActTest.SetFirstRowFixed(false);
    ActTest.SetColumnWid(50, 25, 25);
    for (W_I16 i = 0; i < 5 && i < ReasaultTemp.size(); i++)
    {
        binTemp = CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][7]);
        if (0x00 != StringToHex(ReasaultTemp[i][8]))
            ActTest.AddOneItem(FxGetStdString(binTemp), FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(ReasaultTemp[i][8])));
        else
            ActTest.AddOneItem(FxGetStdString(binTemp));
    }

    DWORD iTimer = 0;
    W_I16 iRet = 0;
    W_I16 iTemp;
    CBinary recv1;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31037005, binAns)))
                return rect;
            if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && 0x10 == binAns[5])
            {
                // 0x5320000000EC        "学习条件不成立"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xEC", 6), DF_MB_OK, DT_LEFT);
                break;
            }
        }
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x01 == binAns[5] || 0x02 == binAns[5]))
        {
            // 0x5320000000CF        "已结束学习"
            if (0x02 == binAns[5])
                ActTest.m_strTitle = FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xCF", 6)).c_str();

            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(binDsCmd, recv1)))
                return rect;
            for (W_I16 i = 0; i < ReasaultTemp.size(); i++)
            {
                binTemp.Clear();
                W_I16 StartPos = 1;
                for (W_I16 j = 0; j < i; j++)
                {
                    StartPos += String2Binary(ReasaultTemp[j][2]).GetByteCount();
                    StartPos += StringToHex(ReasaultTemp[j][6]);
                }
                StartPos += String2Binary(ReasaultTemp[i][2]).GetByteCount();
                iTemp = StringToHex(ReasaultTemp[i][6]);
                for (W_I16 j = 0; j < iTemp && StartPos < recv1.GetByteCount(); j++)
                    binTemp += recv1[StartPos++];
                strTemp = SubaDataCanCal(binTemp, ReasaultTemp[i]);
                if (strTemp.length() <= 0)
                    strTemp = FxGetStdString(CBinary("\x53\x4D\x0E\x58\x16\x20", 6));
                ActTest.SetItemValue(i, strTemp);
            }
            do { iRet = ActTest.ShowCtrl(); } while (0x02 == binAns[5] && -1 != iRet);
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
        if (-1 == iRet)
            break;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31027005, binAns)))
        return rect;
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 防盗锁系统
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02071a20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binTemp, binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin31015004("\x31\x01\x50\x04", 4);
    CBinary bin31035004("\x31\x03\x50\x04", 4);

    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))   // 安全访问
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31015004, binAns)))
        return rect;

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 150;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31035004, binAns)))
                return rect;
        }
        // 0x5320000000C4        "通信线路检查\n正在进行中……"
        if (binAns.GetByteCount() > 5 && 0x00 == binAns[4] && (0x02 == binAns[5] || 0x10 == binAns[5]))
        {
            // 0x5320000000C6        "通信线路未短路"
            // 0x532000000080        "执行条件不成立"
            if (0x02 == binAns[5])
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC6", 6), DF_MB_OK, DT_CENTER);
            else
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x80", 6), DF_MB_OK, DT_CENTER);
            break;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC4", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            // 0x5320000000C5        "已终止通信线路检查"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\xC5", 6), DF_MB_OK, DT_CENTER);
            break;
        }
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 清除AT记忆值
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_12637620(CBinary binMenu)
{

    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200002000E        "如已清除自动变速器（AT）学习，抬起车辆后需再次学习自动变速器（AT）"
    // 0x53200002000F        "是否清除所有自动变速器（AT）学习？"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x0E", 6), DF_MB_YESNO, DT_CENTER))
        return rect;
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x0F", 6), DF_MB_YESNO, DT_CENTER))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;

    CBinary bin3101("\x31\x01\xff\x00\x00\x01", 6);
    CBinary bin3103("\x31\x03\xff\x00", 4);
    CBinary binAns;

    DWORD iTimer = 0;
    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101, binAns)))
            return rect;
        iTimer = 0;
        while (1)
        {
            if (iTimer < GetTickCount())
            {
                iTimer = GetTickCount() + 110;
                if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103, binAns)))
                    return rect;
            }
            if (binAns.GetByteCount() < 6 || 0x00 != binAns[4] || (0x01 != binAns[5] && 0x02 != binAns[5]))
            {
                // 0x532000020010        "未清除自动变速器（AT）学习。\n是否再试一次？"
                if (DF_IDYES == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x10", 6), DF_MB_YESNO, DT_LEFT))
                    break;
                return rect;
            }
            else if (0x00 == binAns[4] && 0x02 == binAns[5])
            {
                // 0x532000020011        "清除已结束。\n关闭点火开关。\n再次进行自动变速器（AT）学习。"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x11", 6), DF_MB_OK, DT_LEFT);
                return rect;
            }
            else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
            {
                return rect;
            }
        }
    }

    return rect;
}


// AWD 开/关切换模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_42637620(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin221051("\x22\x10\x51", 3);
    CBinary bin2E1055("\x2E\x10\x55\x00", 4);
    CBinary bin221056("\x22\x10\x56", 3);
    CBinary bin221055("\x22\x10\x55", 3);
    CBinary bin2E10565A("\x2E\x10\x56\x5A", 4);
    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221051, binAns)))
            return rect;
        if (binAns.GetByteCount() > 3 && 0x00 == binAns[3])
            break;
        // 0x532000020045        "停车，打开点火开关"
        if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x45", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
            return rect;
    }
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin2E1055, binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221056, binAns)))
        return rect;
    // 0x53200002004B        "现在，车辆是否从全轮驱动（AWD）切换到打开（ON）？"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x4B", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    // 0x53200002004C        "整车验证后始终回到全轮驱动。"
    if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x4C", 6), DF_MB_OKCANCEL, DT_CENTER))
        return rect;
    CBinary bin221219("\x22\x12\x19", 3);
    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221219, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() > 3 && 0xff == binAns[3])
            break;
        // 0x532000020047        "设为P挡"
        if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x47", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
            return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221051, binAns)))
        return rect;
    if (binAns.GetByteCount() < 4 || 0x00 != binAns[3])
    {
        // 0x532000020049        "发动机关闭状态下打开点火开关。\n请再试一次"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x49", 6), DF_MB_OK, DT_CENTER);
        m_pNetLayer->SendReceive(ExitCmd);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221056, binAns)))
        return rect;
    if (binAns.GetByteCount() < 4 || 0x00 != binAns[3])
    {
        // 0x53200002004D        "切换失败！请重试。"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x4D", 6), DF_MB_OK, DT_CENTER);
        m_pNetLayer->SendReceive(ExitCmd);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin2E10565A, binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221055, binAns)))
        return rect;
    if (binAns.GetByteCount() < 4 || 0xff != binAns[3])
    {
        // 0x53200002004D        "切换失败！请重试。"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x4D", 6), DF_MB_OK, DT_CENTER);
    }
    else if (CErrorCode::EC_SUCCESS == (rect = SendAndRecive(bin221056, binAns)))
    {
        // 0x53200002004A        "已切换到全轮驱动（AWD）。如要返回到前置前驱（FF），请再次执行基本模式"
        if (binAns.GetByteCount() > 3 && 0x00 == binAns[3])
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x4A", 6), DF_MB_OK, DT_CENTER);
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// VSC（VDC）中间值设置模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02061b20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    // 0x53200002003C        "车辆稳定性控制系统（VSC）（VDC）定心模式\n角度传感器和横向重力感应器零点位置"
    // 0x53200002003D        "车辆停在水平面上，车身摆正，按下【是】按钮"
    /*FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x3C", 6), DF_MB_NOBUTTON, DT_LEFT);
    Sleep(2000);*/
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x3D", 6), DF_MB_YESNO, DT_CENTER))//车辆停在水平面上，车身摆正，按下【是】按钮
        return rect;

    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin1003("\x10\x03", 2);
    CBinary bin221029("\x22\x10\x29", 3);
    CBinary bin22102D("\x22\x10\x2D", 3);
    CBinary bin22102C("\x22\x10\x2C", 3);
    CBinary bin31010024("\x31\x01\x00\x24", 4);
    CBinary bin31030024("\x31\x03\x00\x24", 4);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221029, binAns)))
        return rect;
    if (binAns.GetByteCount() >= 3 && (binAns[0] != 0x62 || binAns[1] != 0x10 || binAns[2] != 0x29))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binAns.GetByteCount() > 4 && binAns[4] > 0x0C)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x3F", 6), DF_MB_OK, DT_CENTER);//转向角传感器的设定零点已矫枉过正。        
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x26", 6), DF_MB_OK, DT_CENTER);//车辆稳定性控制系统（VSC）传感器无法定心
        m_pNetLayer->SendReceive(ExitCmd);
        return rect;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin22102D, binAns)))
        return rect;
    if (binAns.GetByteCount() >= 3 && (binAns[0] != 0x62 || binAns[1] != 0x10 || binAns[2] != 0x2D))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binAns.GetByteCount() > 3 && binAns[3] > 0x11)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x30", 6), DF_MB_OK, DT_CENTER);//横向G传感器的0点设置超过了修正限度    
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x26", 6), DF_MB_OK, DT_CENTER);//车辆稳定性控制系统（VSC）传感器无法定心
        m_pNetLayer->SendReceive(ExitCmd);
        return rect;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin22102C, binAns)))
        return rect;
    if (binAns.GetByteCount() >= 3 && (binAns[0] != 0x62 || binAns[1] != 0x10 || binAns[2] != 0x2C))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binAns.GetByteCount() > 3 && binAns[3] > 0x11)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x30", 6), DF_MB_OK, DT_CENTER);//横向G传感器的0点设置超过了修正限度    
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x26", 6), DF_MB_OK, DT_CENTER);//车辆稳定性控制系统（VSC）传感器无法定心
        m_pNetLayer->SendReceive(ExitCmd);
        return rect;
    }

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31010024, binAns)))
        return rect;
    if (binAns.GetByteCount() < 4)
    {
        // 0x532000020040        "车辆稳定性控制系统（VSC）传感器定心异常"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x40", 6), DF_MB_OK, DT_CENTER);
        return rect;
    }
    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31030024, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() >= 3 && (binAns[0] != 0x71 || binAns[1] != 0x03 || binAns[2] != 0x00))
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binAns.GetByteCount() >= 4 && 0x02 == binAns[3])
        {
            //0x53,0x20,0x00,0x00,0x01,0x32                "VSC（VDC）中间值设置模式启动"
            //0x53,0x20,0x00,0x00,0x01,0x33                "VSC（VDC）中间值设置模式结束"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x32", 6), DF_MB_NOBUTTON, DT_CENTER);
            Sleep(2000);
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x33", 6), DF_MB_OK, DT_CENTER);
            m_pNetLayer->SendReceive(ExitCmd);
            break;
        }
        else if (binAns.GetByteCount() < 4 || 0x01 != binAns[3])
        {
            // 0x532000000026        "车辆稳定性控制系统（VSC）传感器无法定心"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x26", 6), DF_MB_OK, DT_CENTER);
            m_pNetLayer->SendReceive(ExitCmd);
            return rect;
        }
        else
            FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_HAVEBUTTON_DRAW_THEN_RETURN | DF_MB_NOBUTTON, DT_CENTER);
    }

    return rect;
}

// ABS 序列控制模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02051b20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary bin1003("\x10\x03", 2);
    CBinary bin22102B("\x22\x10\x2B", 3);
    CBinary bin31010025("\x31\x01\x00\x25", 4);
    CBinary bin31030025("\x31\x03\x00\x25", 4);
    // 0x532000000019        "用力踩制动踏板"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x19", 6), DF_MB_NOBUTTON, DT_CENTER);
    Sleep(2000);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin22102B, binAns)))
        return rect;

    // 0x53200000001A        "按【OK】"
    while (DF_IDOK == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x1A", 6), DF_MB_OKCANCEL, DT_CENTER))
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))
            return rect;
        binAns = SubaruCanSendReceive(bin31010025);
        if (0 >= binAns.GetByteCount())
        {
            FxShowMessageBox(binMenu, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return rect;
        }
        if (binAns.GetByteCount() < 4 || 0x7f == binAns[0])
        {
            // 0x53200000001B        "无法启动ABS序列控制模式"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x1B", 6), DF_MB_OK, DT_LEFT);
            return rect;
        }
        // 0x53000001010E        "序列控制模式即将开启"
        // 0x53000001010F        "正在启动左前和右后电磁阀"
        // 0x530000010110        "正在启动右前和左后电磁阀"
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x0E", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x0F", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x10", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        DWORD iTimer = 0;
        while (1)
        {
            if (iTimer < GetTickCount())
            {
                iTimer = GetTickCount() + 110;
                if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31030025, binAns)))
                    return rect;
            }
            if (binAns.GetByteCount() < 6 || (0x01 != binAns[5] && 0x02 != binAns[5]))
            {
                // 0x532000000022        "功能检测无法顺利完成，请再试一次。"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x22", 6), DF_MB_OK, DT_CENTER);
                return rect;
            }
            else if (0x02 == binAns[5])
            {
                // 0x532000000083        "ABS序列控制将结束"
                // 0x532000000084        "是否再试一次？"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x83", 6), DF_MB_NOBUTTON, DT_CENTER);
                Sleep(5000);
                if (DF_IDYES == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x84", 6), DF_MB_YESNO, DT_CENTER))
                    break;
                return rect;
            }
        }
    }

    return rect;
}

// VDC 检查模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02041b20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary bin1003("\x10\x03", 2);
    CBinary bin31010026("\x31\x01\x00\x26", 4);
    CBinary bin31030026("\x31\x03\x00\x26", 4);

    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x14", 6), DF_MB_NOBUTTON, DT_CENTER);//车辆动态控制(VDC) 功能检查模式
    Sleep(2000);

    // 0x532000020027        "按【OK】\n如不能启动发动机，关闭点火开关，重新启动发动机。\n并重新启动手动换挡（TOOL）。"
    while (DF_IDOK == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x27", 6), DF_MB_OKCANCEL, DT_LEFT))
    {
        if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))
            return rect;
        binAns = SubaruCanSendReceive(bin31010026);
        if (0 >= binAns.GetByteCount())
        {
            FxShowMessageBox(binMenu, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return rect;
        }
        if (binAns.GetByteCount() < 4 || 0x7f == binAns[0])
        {
            // 0x530000010150        "序列控制模式无法启动"
            FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x50", 6), DF_MB_OK, DT_LEFT);
            return rect;
        }
        // 0x53000001010E        "序列控制模式即将开启"
        // 0x530000010117        "正在检查左前/右后线路"
        // 0x530000010152        "正在检查右前/左后线路"
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x0E", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x17", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x52", 6), DF_MB_NOBUTTON, DT_CENTER);
        Sleep(2000);
        DWORD iTimer = 0;
        while (1)
        {
            if (iTimer < GetTickCount())
            {
                iTimer = GetTickCount() + 110;
                if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31030026, binAns)))
                    return rect;
            }
            if (binAns.GetByteCount() < 6 || (0x01 != binAns[5] && 0x02 != binAns[5]))
            {
                // 0x532000000022        "功能检测无法顺利完成，请再试一次。"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x22", 6), DF_MB_OK, DT_CENTER);
                return rect;
            }
            else if (0x02 == binAns[5])
            {
                // 0x532000020028        "车辆动态控制系统（VDC）功能检查即将结束。"
                // 0x532000000084        "是否再试一次？"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x28", 6), DF_MB_NOBUTTON, DT_CENTER);
                Sleep(5000);
                if (DF_IDYES == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x84", 6), DF_MB_YESNO, DT_CENTER))
                    break;
                return rect;
            }
        }
    }

    return rect;
}

// 刹车维护模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02021b20(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary bin19022A("\x19\x02\x2A", 3);
    CBinary bin1003("\x10\x03", 2);
    CBinary bin31010122("\x31\x01\x01\x22", 4);
    CBinary bin31030122("\x31\x03\x01\x22", 4);

    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin19022A, binAns)))
        return rect;
    // 0x532000000008        "此模式可用于检测/更换/维修刹车片/盘式转子/制动钳。\n启动此模式时，先抬起车辆确保发动机已停止运转，点击【是】。 \n点击【否】返回到'工作支持菜单'。"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x08", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    // 0x532000000009        "启动制动保养模式。\n期间电动驻车制动器将自动运行，确保电动驻车制动器未制动，且当前无任何制动动作。\n点击【否】返回到主菜单。"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x09", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))
        return rect;
    binAns = SubaruCanSendReceive(bin31010122);
    if (0 >= binAns.GetByteCount())
    {
        FxShowMessageBox(binMenu, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return rect;
    }
    if (binAns.GetByteCount() < 4 || 0x7f == binAns[0])
    {
        // 0x53200000000A        "无法启动制动保养模式。\n确认以下几点情况后再试一次。\n是否未启用电动驻车制动器？\n制动组件是否已妥善安装？\n线束连接器是否已连接好？\n发动机是否在运行？"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x0A", 6), DF_MB_OK, DT_LEFT);
        return rect;
    }

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin31030122, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() < 6 || 0x00 != binAns[4] || (0x01 != binAns[5] && 0x02 != binAns[5]))
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x0A", 6), DF_MB_OK, DT_LEFT);
            break;
        }
        else if (0x02 == binAns[5])
        {
            // 0x53200000000B        "制动系统保养模式已启动。\n保养模式下，电动驻车制动开关已禁用。\n另外，驻车警告灯已打开，故障码C1984已保存。"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x00\x0B", 6), DF_MB_OK, DT_LEFT);
            break;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_HAVEBUTTON_DRAW_THEN_RETURN | DF_MB_CANCEL, DT_LEFT))
        {
            break;
        }
    }

    return rect;
}

// 模式移动
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_d2647620(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary bin1001("\x10\x01", 2);
    CBinary bin1003("\x10\x03", 2);
    CBinary binAns;

    SubaruCanSendReceive(bin1001);
    // 0x53200002001A        "当前模式：正常模式\n切换至检测模式。\n点击【OK】按钮。\n提示：\n执行此功能时可能会删除已保存的诊断码和冻结数据。"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x1A", 6), DF_MB_OK, DT_LEFT);
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin1003, binAns)))
        return rect;
    // 0x53200002001B        "已转换到检测模式"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x1B", 6), DF_MB_OK, DT_CENTER);

    return rect;
}

// 擦除无钥ID码
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_32667620(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary bin3101000E("\x31\x01\x00\x0E", 4);
    CBinary bin3103000E("\x31\x03\x00\x0E", 4);
    CBinary bin3102000E("\x31\x02\x00\x0E", 4);

    // 0x532000020037        "清除无匙ID！\n是否确定清除ID？\n关闭点火开关，清除之前拔出钥匙。"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x37", 6), DF_MB_YESNO, DT_LEFT))
        return rect;

    // 0x532000020032        "确认点火开关已关闭、钥匙已拔出。"
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x32", 6), DF_MB_NOBUTTON, DT_CENTER);
    Sleep(2000);
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101000E, binAns)))
        return rect;

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103000E, binAns)))
                return rect;
        }
        // 0x53200002003A        "清除已完成"
        // 0x532000020039        "正在清除无匙ID\n请勿打开点火"
        if (binAns.GetByteCount() > 5 && 0x02 == binAns[5])
        {
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3102000E, binAns)))
                return rect;
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x3A", 6), DF_MB_OK, DT_CENTER);
            return rect;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x39", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
    }

    return rect;
}

// 无钥匙 ID 注册
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_42667620(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin221043("\x22\x10\x43", 3);
    CBinary bin3101000D("\x31\x01\x00\x0D", 4);
    CBinary bin3103000D("\x31\x03\x00\x0D", 4);
    CBinary bin3102000D("\x31\x02\x00\x0D", 4);
    CBinary bin2210281077("\x22\x10\x28\x10\x77", 5);
    // 0x532000020031        "是否注册无匙ID？\n关闭点火开关，注册前拔出钥匙。"
    if (DF_IDYES != FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x31", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;

    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin221043, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() < 4)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_ECU_REFUSE;
        }
        if (0x00 == binAns[3])
        {
            break;
        }
        else
        {
            // 0x532000020032        "确认点火开关已关闭、钥匙已拔出。"
            if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x32", 6), DF_MB_HAVEBUTTON_DRAW_THEN_RETURN | DF_MB_CANCEL, DT_LEFT))
                return rect;
        }
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101000D, binAns)))
        return rect;

    if (binAns.GetByteCount() < 3)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_ECU_REFUSE;
    }
    iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103000D, binAns)))
                return rect;
        }
        // 0x532000020033        "正在切换到注册模式……\n确认点火已关闭，钥匙已拔出。"
        if (binAns.GetByteCount() < 6)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_ECU_REFUSE;
        }
        else if (0x02 == binAns[5])
        {
            break;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x33", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            return rect;
        }
    }

    DWORD ExitTimer = GetTickCount() + 180000;
    // 0x532000000119        "处于注册模式\n注册号= "
    // 0x53200000011A        "\n按压钥匙上的锁车按钮实现注册，然后按压解锁按钮\n3分钟内自动完成注册"
    string DisTxt1 = FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x19", 6));
    string DisTxt2 = FxGetStdString(CBinary("\x53\x20\x00\x00\x01\x1A", 6));
    char regNum = 0;
    iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin2210281077, binAns)))
                return rect;
        }
        if (binAns.GetByteCount() < 7)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_ECU_REFUSE;
        }
        if (0x00 == binAns[6])
        {
            // 0x532000020035        "请再从头开始"
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x35", 6), DF_MB_OK, DT_LEFT);
            return rect;
        }
        else if (0x04 <= binAns[3])
        {
            regNum = 0x34;
        }
        else
        {
            regNum = binAns[3] + 0x30;
        }
        if (ExitTimer < GetTickCount())
        {
            return rect;
        }
        else if (DF_IDOK == FxShowMessageBox(FxGetStdString(binMenu), DisTxt1 + regNum + DisTxt2, DF_MB_OK | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT))
        {
            break;
        }
    }
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3102000D, binAns)))
        return rect;

    iTimer = 0;
    FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x18", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103000D, binAns)))
                return rect;
        }
        // 0x532000000118        "注册模式结束……"
        if (binAns.GetByteCount() < 6)
        {
            FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_ECU_REFUSE;
        }
        else if (0x02 == binAns[5])
        {
            break;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x00\x01\x18", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT))
        {
            return rect;
        }
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 重新调零
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_c2607620(CBinary binMenu)
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    CBinary binAns;
    CBinary ExitCmd("\x10\x01", 2);
    CBinary bin1902AF("\x19\x02\xAF", 3);
    CBinary bin3101f00d("\x31\x01\xf0\x0d", 4);
    CBinary bin3103f00d("\x31\x03\xf0\x0d", 4);

    // 0x532000010006        "参阅《维修手册》。\n\n添加'检查车辆能成功实现归零的条件'"
    // 0x53200001000E        "归零\n\n将乘客座椅调整到《维修手册》中说明的状态。"
    // 0x532000010008        "解开乘客座椅安全带"
    // 0x532000010009        "归零\n\n确认座椅温度在0到40℃（即32到104℉）之间"
    // 0x53200001000A        "归零\n\n清空乘客座椅。\n关闭点火，10秒内再打开点火。"
    // 0x53000001016B        "正在运行...\n\n\n请稍候，勿触摸车辆"
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x06", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    binAns = SubaruCanSendReceive(bin1902AF);
    if (binAns.GetByteCount() > 0 && binAns[0] != 0x59)
    {
        FxShowMessageBox(binMenu, Subaru_TXT_DONTSUPORTFUNCNOW);
        return rect;
    }
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x0E", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x08", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x09", 6), DF_MB_YESNO, DT_LEFT))
        return rect;
    if (DF_IDNO == FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x0A", 6), DF_MB_YESNO, DT_LEFT))
        return rect;

    CMessageBoxCtrl::SetBusyStatus(true);
    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x6B", 6), DF_MB_NOBUTTON, DT_LEFT);
    Sleep(15000);
    CMessageBoxCtrl::SetBusyStatus(false);

    if (binAns.GetByteCount() > 3)
    {
        // 0x53200001000B        "检查诊断码\n\n出现一些故障\n请参阅《维修手册》。\n按【是】结束"
        FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x0B", 6), DF_MB_OK, DT_LEFT);
        return rect;
    }
    if (CErrorCode::EC_SUCCESS != (rect = ProtectCal()))
        return rect;
    if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3101f00d, binAns)))
        return rect;
    DWORD iTimer = 0;
    while (1)
    {
        if (iTimer < GetTickCount())
        {
            iTimer = GetTickCount() + 110;
            if (CErrorCode::EC_SUCCESS != (rect = SendAndRecive(bin3103f00d, binAns)))
                return rect;
        }
        // 0x53200001000C        "归零失败\n\n参阅《维修手册》。\n按【是】结束"
        // 0x53200001000F        "已成功完成归零\n\n按【是】结束"
        // 0x530000000012        "\n请等待"
        if (binAns.GetByteCount() < 6 || 0x00 != binAns[4] || (0x01 != binAns[5] && 0x02 != binAns[5]))
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x0C", 6), DF_MB_YES, DT_LEFT);
            break;
        }
        else if (0x02 == binAns[5])
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x01\x00\x0F", 6), DF_MB_YES, DT_LEFT);
            break;
        }
        else if (DF_IDCANCEL == FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x00\x00\x12", 6), DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER))
        {
            break;
        }
    }
    m_pNetLayer->SendReceive(ExitCmd);

    return rect;
}

// 怠速控制
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_01844220(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRet, Flag = 0;
    W_U32            iValue, iValue2;
    CActTestCtrl    actTest;
    CBinary            binRecv;
    CBinary            binStartCmd("\x2F\x41\x10\x03\x00\x00", 6);
    CBinary            binStopCmd("\x2F\x41\x10\x00", 4);
    string            strValue, strValue1, strValue2;
    char            Temp[10];

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = ProtectCal()))
        return ecRect;

    actTest.InitCtrl(binMenu);
    actTest.SetColumnWid(50, 25, 25);
    actTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x02\x00\x52", 6)), FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF6", 6)));//目标值:
    actTest.AddOneItem(FxGetStdString(CBinary("\x53\x00\x00\x00\x02\x0C", 6)));//状态数据
    actTest.AddOneItem(FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xD8", 6)), FxGetStdString(CBinary("\x53\x20\x00\x00\x00\xF6", 6)));//发动机转速
    actTest.AddOneBtn(CBinary("\x53\x4D\x0F\x72\x16\x20", 6));//0x53,0x4D,0x0F,0x72,0x16,0x20    "+"
    actTest.AddOneBtn(CBinary("\x53\x4D\x02\x0C\x16\x20", 6));//0x53,0x4D,0x02,0x0C,0x16,0x20    "-"
    actTest.AddOneBtn(CBinary("\x53\x4D\x62\x07\x16\x20", 6));//0x53,0x4D,0x62,0x07,0x16,0x20    "开始"
    actTest.AddOneBtn(CBinary("\x53\x4D\x6A\x1B\x16\x20", 6));//0x53,0x4D,0x6A,0x1B,0x16,0x20    "停止"
    strValue1 = "---";
    strValue2 = "0";
    iValue = 1000;
    actTest.SetItemValue(0, "1000");
    actTest.SetItemValue(1, strValue1);
    actTest.SetItemValue(2, strValue2);
    actTest.SetBtnStatus(3, false);

    while (1)
    {
        sprintf(Temp, "%d", iValue);
        strValue = Temp;
        actTest.SetItemValue(0, strValue);

        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x00\x0C", 3), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv.GetByteCount() == 3)
            iValue2 = 0;
        else if (binRecv.GetByteCount() == 4)
            iValue2 = binRecv[3] * 256;
        else
            iValue2 = binRecv[3] * 256 + binRecv[4];
        sprintf(Temp, "%d", iValue2 / 4);
        strValue2 = Temp;
        actTest.SetItemValue(2, strValue2);

        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x30\x03", 3), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv[4] == 1)
            strValue1 = FxGetStdString(CBinary("\x53\x20\x00\x02\x00\x53", 6));//0x53,0x20,0x00,0x02,0x00,0x53    "执行中"
        else if (binRecv[4] == 2)
            strValue1 = FxGetStdString(CBinary("\x53\x4D\x67\x30\x16\x20", 6));//0x53,0x4D,0x67,0x30,0x16,0x20    "正常结束"
        actTest.SetItemValue(1, strValue1);

        if (Flag)
        {
            if (binRecv[4] != 1 && binRecv[4] != 2)
            {
                //0x53,0x20,0x00,0x02,0x00,0x54                "不能满足活性评价的操作状态。\n状态: 未执行"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x54", 6), DF_MB_OK, DT_LEFT);
                return ecRect;
            }
        }

        if (iValue <= 500)
            actTest.SetBtnStatus(1, false);
        else
            actTest.SetBtnStatus(1, true);
        if (iValue >= 2000)
            actTest.SetBtnStatus(0, false);
        else
            actTest.SetBtnStatus(0, true);

        iRet = actTest.ShowCtrl();
        if (iRet == -1)
            return ecRect;

        if (iRet == 0)
            iValue += 50;
        else if (iRet == 1)
            iValue -= 50;
        else if (iRet == 2)
        {
            actTest.SetBtnStatus(2, false);
            actTest.SetBtnStatus(3, true);
            actTest.ShowCtrl();

            binStartCmd.SetByteAt(4, ((iValue * 4) & 0xFF00) >> 8);
            binStartCmd.SetByteAt(5, ((iValue * 4) & 0xFF));
            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binStartCmd, binRecv)))
                return ecRect;
            if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x6F)
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_COMMUNICATION;
            }
        }
        else if (iRet == 3)
        {
            Flag = 1;
            actTest.SetBtnStatus(2, true);
            actTest.SetBtnStatus(3, false);
            actTest.ShowCtrl();

            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binStopCmd, binRecv)))
                return ecRect;
            if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x6F)
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_COMMUNICATION;
            }
        }
    }

    return ecRect;
}

// 固定怠速点火定时
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_00844220(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRet, Flag = 0;
    CActTestCtrl    actTest;
    CBinary            binRecv;
    string            strValue1, strValue2;
    char            Temp[10];

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = ProtectCal()))
        return ecRect;

    actTest.InitCtrl(binMenu);
    actTest.SetColumnWid(50, 25, 25);
    actTest.AddOneItem(FxGetStdString(CBinary("\x53\x4D\xA8\x15\x16\x20", 6)));//固定怠速点火定时
    actTest.AddOneItem(FxGetStdString(CBinary("\x53\x4D\x14\x0A\x16\x20", 6)), FxGetStdString(CBinary("\x53\x4D\x08\x16\x16\x20", 6)));//点火定时 1
    actTest.AddOneBtn(CBinary("\x53\x4D\x62\x07\x16\x20", 6));//0x53,0x4D,0x62,0x07,0x16,0x20    "开始"
    actTest.AddOneBtn(CBinary("\x53\x4D\x6A\x1B\x16\x20", 6));//0x53,0x4D,0x6A,0x1B,0x16,0x20    "停止"
    strValue1 = "---";
    strValue2 = "0";
    actTest.SetItemValue(0, strValue1);
    actTest.SetItemValue(1, strValue2);
    actTest.SetBtnStatus(1, false);

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x00\x0E", 3), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x62 || binRecv[1] != 0x00 || binRecv[2] != 0x0E)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv.GetByteCount() >= 4)
        {
            sprintf(Temp, "%d", (int)(-64 + binRecv[3] * 0.5));
            strValue2 = Temp;
            actTest.SetItemValue(1, strValue2);
        }

        if (Flag)
        {
            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x31\x03\x41\x00", 4), binRecv)))
                return ecRect;
            if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x71 || binRecv[1] != 0x03 || binRecv[2] != 0x41)
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_COMMUNICATION;
            }
            if ((binRecv.GetByteCount() >= 6) && (binRecv[5] == 1 || binRecv[5] == 2))
            {
                if (binRecv[5] == 1)
                    strValue1 = FxGetStdString(CBinary("\x53\x20\x00\x02\x00\x53", 6));//0x53,0x20,0x00,0x02,0x00,0x53    "执行中"
                else if (binRecv[5] == 2)
                    strValue1 = FxGetStdString(CBinary("\x53\x4D\x1A\x3A\x16\x20", 6));//0x53,0x4D,0x1A,0x3A,0x16,0x20    "停止"
                actTest.SetItemValue(0, strValue1);
            }
            else
            {
                //0x53,0x20,0x00,0x02,0x00,0x54                "不能满足活性评价的操作状态。\n状态: 未执行"
                FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x54", 6), DF_MB_OK, DT_LEFT);
                return ecRect;
            }
        }

        iRet = actTest.ShowCtrl();
        if (iRet == -1)
        {
            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x10\x01", 2), binRecv)))
                return ecRect;
            return ecRect;
        }

        if (iRet == 0)
        {
            Flag = 1;
            actTest.SetBtnStatus(0, false);
            actTest.SetBtnStatus(1, true);
            actTest.ShowCtrl();

            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x31\x01\x41\x00", 4), binRecv)))
                return ecRect;
            if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x71 || binRecv[1] != 0x01 || binRecv[2] != 0x41)
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_COMMUNICATION;
            }
        }
        else if (iRet == 1)
        {
            Flag = 0;
            actTest.SetBtnStatus(0, true);
            actTest.SetBtnStatus(1, false);
            actTest.SetItemValue(0, "---");
            actTest.ShowCtrl();
            if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x31\x02\x41\x00", 4), binRecv)))
                return ecRect;
            if (binRecv.GetByteCount() < 3 || binRecv[0] != 0x71 || binRecv[1] != 0x02 || binRecv[2] != 0x41)
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                return CErrorCode::EC_COMMUNICATION;
            }
        }
    }

    return ecRect;
}

// 自动变速器记忆模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_22637620(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iKey, iLength, iPos;
    CBinary            binRecv;
    string            strValue, strShow, strTemp, strValue1, strValue2, strValue3;
    char            Temp[50];

    iKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x04\x0A", 6), DF_MB_OKCANCEL, DT_LEFT);
    if (iKey == DF_IDCANCEL)
        return ecRect;

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x4C", 3), binRecv)))
        return ecRect;

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x4D\x10\x4E\x10\x4F\x10\xD2", 9), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() <= 0 || binRecv[0] != 0x62)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv.GetByteCount() < 13)
        {
            iLength = binRecv.GetByteCount();
            for (int i = iLength; i < 13; i++)
                binRecv += CBinary("\x00", 1);
        }

        sprintf(Temp, "%d", -50 + binRecv[6]);
        strValue1 = Temp;
        sprintf(Temp, "%d", -50 + binRecv[9]);
        strValue2 = Temp;
        strValue2 += "℃";

        strShow = FxGetStdString(CBinary("\x53\x4D\x10\x02\x16\x20", 6));//"加热发动机直到ATF 温度达到%s — %s%s,\n或在ATF变冷之后再执行AT记忆。"
        iPos = strShow.find("%s");
        if (iPos >= 0)
            strShow.replace(iPos, 2, strValue1);
        iPos = strShow.find("%s%s");
        if (iPos >= 0)
            strShow.replace(iPos, 4, strValue2);
        strTemp = FxGetStdString(CBinary("\x53\x4D\xA2\x58\x16\x20", 6));//"当前ATF温度  %d ℃"
        sprintf(Temp, strTemp.c_str(), -50 + binRecv[12]);
        strShow += "\n";
        strShow += Temp;
        iKey = FxShowMessageBox(FxGetStdString(binMenu), strShow, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
        if (iKey == DF_IDCANCEL)
            return ecRect;

        Sleep(150);
        if (binRecv[3] == 0xFF)
            break;
    }

    iKey = FxShowMessageBox(binMenu, CBinary("\x53\x4D\x66\x56\x16\x20", 6), DF_MB_OKCANCEL, DT_LEFT);//"提起车身置于停车刹车状态。"
    if (iKey == DF_IDCANCEL)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = ProtectCal()))
        return ecRect;

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x31\x01\xFF\x00\x00\x01", 6), binRecv)))//31 01 FF 00 00 01
        return ecRect;

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x31\x03\xFF\x00", 4), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() <= 0 || binRecv[0] != 0x71)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv.GetByteCount() < 6)
        {
            iLength = binRecv.GetByteCount();
            for (int i = iLength; i < 6; i++)
                binRecv += CBinary("\x00", 1);
        }

        if (0x00 != binRecv[4])
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x4D\x62\x56\x16\x20", 6), DF_MB_YESNO, DT_LEFT);//AT记忆未清除。\n再次尝试吗？
            break;
        }
        else if (0x00 == binRecv[4] && 0x02 == binRecv[5])
            break;
        else
            FxShowMessageBox(binMenu, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x12\x27", 3), binRecv)))
        return ecRect;
    if (binRecv[0] != 0x62)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    while (1)
    {
        binRecv = m_pNetLayer->SendReceive(CBinary("\x22\x12\x27", 3));
        if (binRecv.GetByteCount() <= 0 || binRecv[0] != 0x62)//提示信息为关闭点火开关，所以没有应答才会进入下一个界面
            break;

        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x6B\x75\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);//关闭点火开关
    }

    FxShowMessageBox(binMenu, CBinary("\x53\x4D\x21\x60\x16\x20", 6), DF_MB_OK, DT_CENTER);//带EyeSight的车辆，起动后请将“防撞”功能关闭。

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x51", 3), binRecv)))
            return ecRect;
        if (binRecv[0] != 0x62)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x4D\x16\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆异常结束。\n请再次从开始进行。
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() >= 4 && binRecv[3] == 0xFF)
            break;

        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x21\x60\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);//带EyeSight的车辆，起动后请将“防撞”功能关闭。
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = ProtectCal()))
        return ecRect;

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x4D\x10\x4E\x10\x4F\x10\xD2", 9), binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() <= 0 || binRecv[0] != 0x62)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        else if (binRecv.GetByteCount() < 13)
        {
            iLength = binRecv.GetByteCount();
            for (int i = iLength; i < 13; i++)
                binRecv += CBinary("\x00", 1);
        }

        sprintf(Temp, "%d", -50 + binRecv[6]);
        strValue1 = Temp;
        sprintf(Temp, "%d", -50 + binRecv[9]);
        strValue2 = Temp;
        strValue2 += "℃";

        strShow = FxGetStdString(CBinary("\x53\x4D\x10\x02\x16\x20", 6));//"加热发动机直到ATF 温度达到%s — %s%s,\n或在ATF变冷之后再执行AT记忆。"
        iPos = strShow.find("%s");
        if (iPos >= 0)
            strShow.replace(iPos, 2, strValue1);
        iPos = strShow.find("%s%s");
        if (iPos >= 0)
            strShow.replace(iPos, 4, strValue2);
        strTemp = FxGetStdString(CBinary("\x53\x4D\xA2\x58\x16\x20", 6));//"当前ATF温度  %d ℃"
        sprintf(Temp, strTemp.c_str(), -50 + binRecv[12]);
        strShow += "\n";
        strShow += Temp;
        iKey = FxShowMessageBox(FxGetStdString(binMenu), strShow, DF_MB_CANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
        if (iKey == DF_IDCANCEL)
            return ecRect;

        Sleep(150);
        if (binRecv[3] == 0xFF)
            break;
    }

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x11\xFA", 3), binRecv)))
            return ecRect;
        if (binRecv[0] != 0x62)
            break;
        if (binRecv.GetByteCount() >= 4 && binRecv[3] == 0xFF)
            break;

        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x0A\x02\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);//持续地踩压制动踏板至底处
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x2E\x10\x54\x5A", 4), binRecv)))
        return ecRect;

    iKey = FxShowMessageBox(binMenu, CBinary("\x53\x4D\x58\x5C\x16\x20", 6), DF_MB_OKCANCEL, DT_LEFT);//换档杆放入P档。\n 此模式为马达特殊模式。\n 检查结束时请确认IG为OFF。
    if (iKey == DF_IDCANCEL)
        return ecRect;

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x12\x04", 3), binRecv)))
            return ecRect;
        if (binRecv[0] != 0x62)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() >= 4 && binRecv[3] == 0xFF)
            break;

        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x0B\x02\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);//在启动发动机后\n当发动机速度达到稳定时选择D档。
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x2E\x10\x4A\x01", 4), binRecv)))
        return ecRect;
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x2E\x10\x4B\x09", 4), binRecv)))
        return ecRect;

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x4B", 3), binRecv)))
            return ecRect;
        if (binRecv[0] != 0x62)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() >= 4 && binRecv[3] == 0x40)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x4D\x16\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆异常结束。\n请再次从开始进行。
            //FxShowMessageBox(binMenu, CBinary("\x53\x4D\x1B\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆正常结束。
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() >= 4 && binRecv[3] == 0x14)
            break;

        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x61\x56\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);//持续踩住刹车踏板。\n\n正在准备AT记忆。
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x50", 3), binRecv)))
        return ecRect;
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x10\x53", 3), binRecv)))
        return ecRect;

    //while (1)
    //{
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(CBinary("\x22\x11\xFA\x10\x4B", 5), binRecv)))
        return ecRect;
    if (binRecv[0] != 0x62)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() >= 7 && binRecv[6] == 0x40)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x16\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆异常结束。\n请再次从开始进行。
        //FxShowMessageBox(binMenu, CBinary("\x53\x4D\x1B\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆正常结束。
        return CErrorCode::EC_COMMUNICATION;
    }
    else
        FxShowMessageBox(binMenu, CBinary("\x53\x4D\x1B\x02\x16\x20", 6), DF_MB_OK, DT_LEFT);//自动变速器记忆正常结束。

    //FxShowMessageBox(binMenu, CBinary("\x53\x4D\x61\x56\x16\x20", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_CENTER);//持续踩住刹车踏板。\n\n正在准备AT记忆。
//}    

    return ecRect;
}

// 力传感器校准模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a64f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd224403("\x22\x44\x03", 3);
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd3101("\x31\x01\x03\x02", 4);
    CBinary            binCmd3103("\x31\x03\x03\x02", 4);

    FxShowMessageBox(binMenu, binMenu, DF_MB_NOBUTTON);
    Sleep(1500);
    //标定力传感器之前，\n于水平方位停止车辆并放置车轮挡块确保安全。\n若情况正常请按\'是\'。
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x99", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd224403, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x62 || binRecv[1] != 0x44 || binRecv[2] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 5)
        binRecv += CBinary("\x00\x00", 2);
    if (binRecv[3] != 0 || binRecv[4] != 0)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x9A", 6));//由于车辆不处于停止状态，无法执行力传感器标定。
        return ecRect;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3101, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x71 || binRecv[1] != 0x01 || binRecv[2] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    W_U32    iBeginTime = GetTickCount();
    W_I16    iCount;
    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3103, binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x71 || binRecv[1] != 0x03 || binRecv[2] != 0x03))
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() < 5)
            binRecv += CBinary("\x00\x00", 2);

        if (binRecv[4] == 0x01)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x9C", 6), DF_MB_OK, DT_LEFT);//力传感器标定完成。\n关闭点火开关。
            return ecRect;
        }
        else if (binRecv[4] == 0x02)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xB5", 6), DF_MB_YES);//力传感器校准中止，请再次执行校准。
            return ecRect;
        }

        //正在进行力传感器标定。\n请稍候（约20秒）
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x9B", 6), DF_MB_NOBUTTON | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN);
        Sleep(150);

        iCount = (GetTickCount() - iBeginTime) / 1000;
        if (iCount >= 20)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x20\x00\x02\x00\x51", 6), DF_MB_OK);//超时
            return ecRect;
        }
    }

    return ecRect;
}

// 停车刹车的违章驾驶模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a54f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd3101("\x31\x01\x03\x01\x01\xC8", 6);
    CBinary            binCmd3E("\x3E", 1);

    FxShowMessageBox(binMenu, binMenu, DF_MB_NOBUTTON);
    Sleep(1500);
    //是否执行磨合驻车制动驱动？\n停止车辆并释放驻车制动。\n按\'是\'或者\'否\'。
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x9E", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3101, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 6 || (binRecv[0] != 0x71 || binRecv[1] != 0x01 || binRecv[2] != 0x03 || binRecv[3] != 0x01 || binRecv[4] != 0x01 || binRecv[5] != 0xC8))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3E, binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 1 || binRecv[0] != 0x7E)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() < 3)
            binRecv += CBinary("\x00\x00", 2);

        if (binRecv[1] != 0 || binRecv[2] != 0)
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return ecRect;
        }

        //是否执行磨合驱动模式？\n磨合驱动模式可在按下驻车开关时执行。\n执行前请确认周围是否安全。\n完成后关闭点火开关。\n关闭点火开关后请按【确定】按钮。
        iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\x9F", 6), DF_MB_OK | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
        if (iRetKey == DF_IDOK)
            return ecRect;
        Sleep(150);
    }

    return ecRect;
}

// 停车刹车拆卸模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a44f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd3001("\x30\x01\x07\x04\x00", 5);

    FxShowMessageBox(binMenu, binMenu, DF_MB_NOBUTTON);
    Sleep(1500);
    //执行驻车制动移除模式时须升起车辆。\n是否执行驻车制动移除？\n按\'是\'或者\'否\'。
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xA1", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3001, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 4 || (binRecv[0] != 0x70 || binRecv[1] != 0x01 || binRecv[2] != 0x07 || binRecv[3] != 0x04))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xA2", 6), DF_MB_OK);//释放驻车线缆直至移除位置。
    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xA3", 6), DF_MB_OK, DT_LEFT);//释放驻车线缆直至移除位置。\n关闭点火开关，然后移除驻车线缆。

    return ecRect;
}

// 离合器结合位置设定
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a24f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd224414("\x22\x44\x14", 3);
    CBinary            binCmd224415("\x22\x44\x15", 3);
    CBinary            binCmd3B03("\x3B\x03\x00\x00", 4);
    string            strShow;
    char            Temp[10];

    FxShowMessageBox(binMenu, binMenu, DF_MB_NOBUTTON);
    Sleep(1500);
    //是否设置离合器接合位置？\n输入接合位置，\n按\'执行\'按钮。
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xA5", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);

    while (1)
    {
        strShow = FxGetStdString(CBinary("\x53\x00\x00\x37\x07\x05", 6));
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd224414, binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x62 || binRecv[1] != 0x44 || binRecv[2] != 0x14))
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() < 5)
            binRecv += CBinary("\x00\x00", 1);
        sprintf(Temp, "%.1f", (binRecv[4] * 2.55 + binRecv[3] * 0.01));
        strShow += Temp;
        strShow += " %";
        strShow += "\n";
        strShow += FxGetStdString(CBinary("\x53\x00\x00\x37\x07\x06", 6));

        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd224415, binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x62 || binRecv[1] != 0x44 || binRecv[2] != 0x15))
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() < 5)
            binRecv += CBinary("\x00\x00", 1);
        sprintf(Temp, "%.1f", (binRecv[4] * 2.55 + binRecv[3] * 0.01));
        strShow += Temp;
        strShow += " %";

        iRetKey = FxShowMessageBox(FxGetStdString(binMenu), strShow, DF_MB_OKCANCEL | DF_MB_HAVEBUTTON_DRAW_THEN_RETURN, DT_LEFT);
        if (iRetKey == DF_IDOK)
            break;
        else if (iRetKey == DF_IDCANCEL)
            return ecRect;
    }

    CInputCtrl    uiInput;
    string        strCaption, strPrompt, strMask, strDefault, strMinValue, strMaxValue, strInputValue;
    char* p;
    DOUBLE        dValue = 0.0;
    W_I32        iValue, iValue1, iValue2;

    while (1)
    {
        uiInput.SetKeyboardShow(true);
        strCaption = FxGetStdString(binMenu);
        strPrompt = FxGetStdString(CBinary("\x53\x00\x00\x01\x01\xA6", 6));
        strMask = "0000";
        strDefault = "00.0";
        strMinValue = "50.1";
        strMaxValue = "79.9";

        iRetKey = uiInput.ShowCtrl(strCaption, strPrompt, strMask, strDefault, strMinValue, strMaxValue);
        if (iRetKey == 7)
            return ecRect;
        strInputValue = uiInput.GetStringValue();
        break;
    }

    dValue = strtod(strInputValue.c_str(), &p);
    iValue = dValue * 100;
    iValue1 = iValue / 256;
    iValue2 = iValue % 256;
    binCmd3B03.SetByteAt(3, iValue1);
    binCmd3B03.SetByteAt(2, iValue2);

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3B03, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x7B || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    strShow = FxGetStdString(CBinary("\x53\x00\x00\x01\x01\xAB", 6));
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd224415, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x62 || binRecv[1] != 0x44 || binRecv[2] != 0x15))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 5)
        binRecv += CBinary("\x00\x00", 1);
    sprintf(Temp, "%.1f", (binRecv[4] * 2.55 + binRecv[3] * 0.01));
    strShow += Temp;
    strShow += " %";
    FxShowMessageBox(FxGetStdString(binMenu), strShow);

    return ecRect;
}

// 离合器传感器校准模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a14f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd224403("\x22\x44\x03", 3);
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd3101("\x31\x01\x03\x03", 4);
    CBinary            binCmd3103("\x31\x03\x03\x03", 4);

    FxShowMessageBox(binMenu, binMenu, DF_MB_NOBUTTON);
    Sleep(1500);
    //执行离合器传感器标定时\n须释放离合器踏板。\n是否执行离合器传感器标定？\n按\'是\'或者\'否\'。
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xAD", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd224403, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x62 || binRecv[1] != 0x44 || binRecv[2] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 5)
        binRecv += CBinary("\x00\x00", 2);
    if (binRecv[3] != 0 || binRecv[4] != 0)
    {
        FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xAE", 6));//由于车辆不处于停止状态，无法执行离合器传感器标定。
        return ecRect;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3101, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x71 || binRecv[1] != 0x01 || binRecv[2] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    while (1)
    {
        if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3103, binRecv)))
            return ecRect;
        if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x71 || binRecv[1] != 0x03 || binRecv[2] != 0x03))
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
            return CErrorCode::EC_COMMUNICATION;
        }
        if (binRecv.GetByteCount() < 5)
            binRecv += CBinary("\x00\x00", 2);

        if (binRecv[4] == 0x01)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xAF", 6), DF_MB_OK, DT_LEFT);//离合器传感器标定完成。\n关闭点火开关。
            return ecRect;
        }
        else if (binRecv[4] == 0x02)
        {
            FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xB0", 6), DF_MB_YES, DT_LEFT);//已取消离合器传感器标定。\n再次执行标定！
            return ecRect;
        }

        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
        Sleep(150);
    }

    return ecRect;
}

// 参数初始化模式
W_ErrorCode CSubaruBaseCanApp::SpecFunsID_02a34f20(CBinary binMenu)
{
    W_ErrorCode        ecRect = CErrorCode::EC_SUCCESS;
    W_I16            iRetKey;
    CBinary            binRecv;
    CBinary            binCmd1003("\x10\x03", 2);
    CBinary            binCmd3B01("\x3B\x01", 2);
    CBinary            binCmd3B04("\x3B\x04\x00", 3);

    //执行参数初始化时须连接所有ECM。\n是否执行参数初始化？\n按\'是\'或者\'否\'
    iRetKey = FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xB2", 6), DF_MB_YESNO, DT_LEFT);
    if (iRetKey == DF_IDNO)
        return ecRect;

    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd1003, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x50 || binRecv[1] != 0x03))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3B01, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 2 || (binRecv[0] != 0x7B || binRecv[1] != 0x01))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }
    if (binRecv.GetByteCount() < 3)
        binRecv += CBinary("\x00", 1);
    if (binRecv[2] != 0)
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    if (CErrorCode::EC_SUCCESS != (ecRect = SendAndRecive(binCmd3B04, binRecv)))
        return ecRect;
    if (binRecv.GetByteCount() < 3 || (binRecv[0] != 0x7B || binRecv[1] != 0x04 || binRecv[2] != 0x00))
    {
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
        return CErrorCode::EC_COMMUNICATION;
    }

    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xB3", 6), DF_MB_NOBUTTON);
    Sleep(5000);
    FxShowMessageBox(binMenu, CBinary("\x53\x00\x00\x01\x01\xB4", 6), DF_MB_YES, DT_LEFT);

    return ecRect;
}
