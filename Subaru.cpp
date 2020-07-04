// Subaru.cpp: implementation of the CSubaru class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Subaru.h"


#include "SubaruBaseAllSystemApp.h"
#include "SubaruBaseQuickTestApp.h"
#include "SubaruBaseOldApp.h"
#include "SubaruBaseKwp1App.h"
#include "SubaruBaseKwp2App.h"
#include "SubaruBaseCanApp.h"

#include "MenuCtrl.h"
#include "SubaruLib.h"
#include "MessageBoxCtrl.h"
#include "DtcScanCtrl.h"
#include "TroubleCodeCtrl.h"
#include "FileManager.h"
#include "arithmetic.h"
#include "TextLibrary.h"
#include "InputCtrl.h"
#include "VehicleSave.h"
#include "EobdAllSystem.h"
#include "SubaruBaseApp.h"
#include "SearchString.h"
#include "EcuInfoCtrl.h"
#include "DataStreamCtrl.h"
#include "SubaruBaseApp.h"

#define Subaru_SEARCH_SYS 0xFF
#define Subaru_QUICKTEST_SYS 0x00

bool m_bIsService = false;
W_I32 m_specFuncID = 0;

CBinaryGroup Ecu_Id_Select;  // 存放筛选后的ECUID
string Country_Ecu_File;  // 对应的ECU文件
string System_Search_Id;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSubaru::CSubaru()
{
    //m_bIsCan = false;
    m_iService = 0;
    isProgramming = false;
}
CSubaru::~CSubaru()
{

}

//void CSubaru::test()
//{
//    CSearchString Subaru_Data;
//    CSearchString MenuComd;
//    
//    Subaru_Data.OpenTabFile(Subaru_Data_CBF);
//    MenuComd.OpenTabFile(Subaru_MenuComd_CBF);
//
//    FILE *pOut;
//    FILE *pSysSet;
//    fopen_s(&pSysSet, "SysNumSet.txt", "r");
//
//    string strTemp;
//    W_I16 cnt = 0;
//    char ch;
//    string SystemName = "";
//
//    while (1)
//    {
//        cnt = fread(&ch, 1, 1, pSysSet);
//
//        if (0 == cnt)
//            break;
//
//        if ('\n' == ch)
//        {
//            vector<string> SearchName;
//            SearchName.push_back(strTemp);
//            vector<vector<string>> SearchResault_Data;
//    //        vector<vector<string>> SearchResault_MenuCmd;
//
//            Subaru_Data.SearchString(SearchResault_Data, FALSE, 0, 0, SearchName);
//    //        MenuComd.SearchString(SearchResault_MenuCmd, FALSE, 0, 0, SearchName);
//
//            fopen_s(&pOut, "out.txt", "a+");
//            for (W_I16 i = 0; i < SearchResault_Data.size(); i++)
//            {
//                fprintf(pOut, "%s\t", SystemName.c_str());
//                strTemp = FxGetStdString(CBinary(Subaru_IDCAN, 2) + String2Binary(SearchResault_Data[i][7]));
//                fprintf(pOut, "%s\t", strTemp.c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][3].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][4].c_str());
//                strTemp = SearchResault_Data[i][1] + SearchResault_Data[i][2];
//                fprintf(pOut, "%s\t", strTemp.c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][5].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][6].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][10].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][14].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][15].c_str());
//                fprintf(pOut, "%s\t", SearchResault_Data[i][16].c_str());
//                fprintf(pOut, "%s\n", SearchResault_Data[i][12].c_str());
//            }
//            fclose(pOut);
//            strTemp = "";
//        }
//        else if ('\t' == ch)
//        {
//            SystemName = strTemp;
//            strTemp = "";
//        }
//        else
//        {
//            strTemp += ch;
//        }
//    }
//
//    fclose(pSysSet);
//
//}


// static W_I32 StringToHex(string src)
// {
//     int iRet;
//     char *p;
//
//     iRet = strtoul(src.data(),&p,16);
//
//     return iRet;
// }
/*-----------------------------------------------------------------------------
      功    能：主程序入口
      参    数：无
      返 回 值：错误代码
      说    明：无
-----------------------------------------------------------------------------*/
//W_ErrorCode CSubaru::MainEntrance()
//{    
//    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
//
//    //W_ErrorCode m_value = ChooseVehicle(); // 车型
//    //if (m_value == 0)
//    //{
//    //    return ecRet;
//    //}
//
//    W_I16            iSelMenuItem = 0;
//    W_I16            iSelected = 0;
//    CMenuCtrl        mnuMenu, ctrl;    // 选择菜单
//    CBinary            binMenu("\x01\x02\x04", 3);
//
//
//
//#ifdef    __NT5600DPF__
//    return DiagSoftware();
//#endif
//    while (1)
//    {
//        ctrl.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x3B",6));
//        ctrl.AddOneItem(CBinary("\x53\x10\x00\x00\x00\x01",6));
//        ctrl.AddOneItem(CBinary("\x53\x10\x00\x00\x00\x02",6));
//
//        iSelected = ctrl.ShowCtrl();
//        if (iSelected == -1)
//        {
//            if(FxShowMessageBox(STD_TTL_MSG_INFORMATION,STD_TXT_MSG_EXITSYSTEM,DF_MB_YESNO)==DF_IDYES)
//                break;
//            else
//                continue;
//        }
//        if (iSelected == 0)     // 其他车型
//        {
//#ifndef __NT5600DEF__
//            while (1)
//            {
//                mnuMenu.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x35",6));
//                mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x60",6));//诊断
//                mnuMenu.AddOneItem(CBinary("\xFF\x07\x00\x00\x00\x40",6));//常用特殊功能
//                mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x63",6));//编程
//                iSelMenuItem = mnuMenu.ShowCtrl();  
//                if (iSelMenuItem == -1)
//                    break;
//                switch(iSelMenuItem)
//                {
//                case 0x00:            // 诊断
//                    m_bIsService = false;
//                    DiagSoftware();    
//                    break;
//                case 0x01:            // 常用特殊功能
//                    m_bIsService = true;
//                    //Service();//
                //HotFunction();
//                    m_bIsService = false;
//                    break;
//                case 0x02:
//                    m_bIsService = true;
//                    isProgramming = true;
//                    Programming();    // 编程
//                    m_bIsService = false;
//                    isProgramming = false;
//                    break;
//                }
//            }
//#else
//            DiagSoftware();
//#endif
//        }
//        if (iSelected == 1)   // BRZ车型的检查
//        {
//            VechileStatus = 2;
//            ecRet = DiagSoftware_BRZ();
//        }
//    }
//    return ecRet;
//}



W_ErrorCode CSubaru::MainEntrance()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    W_I16            iSelMenuItem = 0;
    //W_I16            iSelected = 0;
    W_I16            ME_selcet, MA_select;
    W_I16            Message_select;
    W_I16            iNum, iNum2;
    W_I16            Message_Country = 0;
    W_I16            Message_Vehicle = 0;
    W_I16            Message_Year = 0;
    W_I16            Message_ME_MA = 0; // 车辆信息
    CMenuCtrl        mnuMenu, ctrl, menuMA, menuME, vehiclemenu, vehicleyear;
    CBinary            binMenu("\x01\x02\x04", 3);
    // 国家：
    CBinary JP = CBinary(Subaru_Country"\x01", 6);  // 日本
    CBinary NA = CBinary(Subaru_Country"\x02", 6);  // 北美
    CBinary EU = CBinary(Subaru_Country"\x03", 6);  // 欧洲
    CBinary ASIA = CBinary(Subaru_Country"\x04", 6);  // 亚洲/大洋洲
    CBinary ME = CBinary(Subaru_Country"\x05", 6);  // 中东
    CBinary AF = CBinary(Subaru_Country"\x06", 6);  // 非洲
    CBinary MA = CBinary(Subaru_Country"\x07", 6); // 中美洲和南美洲

    // 中东：
    CBinary BL = CBinary(Subaru_ME_MA_Country"\x0D", 6); // 巴林王国
    CBinary SP = CBinary(Subaru_ME_MA_Country"\x0E", 6); // 塞浦路斯共和国
    CBinary AL = CBinary(Subaru_ME_MA_Country"\x0F", 6); // 阿拉伯埃及共和国
    CBinary YS = CBinary(Subaru_ME_MA_Country"\x10", 6); // 以色列国
    CBinary YD = CBinary(Subaru_ME_MA_Country"\x11", 6); // 约旦哈希姆王国
    CBinary KW = CBinary(Subaru_ME_MA_Country"\x12", 6); // 科威特国
    CBinary LB = CBinary(Subaru_ME_MA_Country"\x13", 6); // 黎巴嫩共和国
    CBinary ML = CBinary(Subaru_ME_MA_Country"\x14", 6); // 摩洛哥王国
    CBinary AM = CBinary(Subaru_ME_MA_Country"\x15", 6); // 阿曼苏丹国
    CBinary KT = CBinary(Subaru_ME_MA_Country"\x16", 6); // 卡塔尔国
    CBinary ST = CBinary(Subaru_ME_MA_Country"\x17", 6); // 沙特阿拉伯王国
    CBinary TE = CBinary(Subaru_ME_MA_Country"\x18", 6); // 土耳其共和国
    CBinary QZ = CBinary(Subaru_ME_MA_Country"\x19", 6); // 阿拉伯联合酋长国
    CBinary QT_E = CBinary(Subaru_ME_MA_Country"\x1A", 6); // 其他

    // 中美洲和南美洲：
    CBinary AT = CBinary(Subaru_ME_MA_Country"\x01", 6); // 安提瓜和巴布达
    CBinary BB = CBinary(Subaru_ME_MA_Country"\x02", 6); // 巴巴多斯
    CBinary BM = CBinary(Subaru_ME_MA_Country"\x03", 6); // 百慕大
    CBinary KM = CBinary(Subaru_ME_MA_Country"\x04", 6); // 开曼群岛
    CBinary DM = CBinary(Subaru_ME_MA_Country"\x05", 6); // 多米尼加联邦
    CBinary GL = CBinary(Subaru_ME_MA_Country"\x06", 6); // 格林纳达
    CBinary YM = CBinary(Subaru_ME_MA_Country"\x07", 6); // 牙买加
    CBinary SJ = CBinary(Subaru_ME_MA_Country"\x08", 6); // 圣基茨
    CBinary SL = CBinary(Subaru_ME_MA_Country"\x09", 6); // 圣卢西亚
    CBinary LN = CBinary(Subaru_ME_MA_Country"\x0A", 6); // 苏里南
    CBinary TL = CBinary(Subaru_ME_MA_Country"\x0B", 6); // 特立尼达和多巴哥
    CBinary QT_A = CBinary(Subaru_ME_MA_Country"\x0C", 6); // 其他

    CBinary binSearchID = CBinary("\x00\x00\x00\x00\x00\x01", 6);  // 车型ID索引
    Vehcilename = CBinary("\x53\x5D", 2);
    //CBinary Vehcilename = CBinary("\x53\x5D",2);   // 车型名称前缀
    CBinary binMainMenu, m_VehicleIndex;

    m_country.Clear();   // 清空避免重复
    //CBinaryGroup m_country,country_ME,country_MA,m_vehicle,m_vehicle_name,m_vehicle_type,m_vehicle_year;
    // 国家：
    m_country.Append(JP);
    m_country.Append(NA);
    m_country.Append(EU);
    m_country.Append(ASIA);
    m_country.Append(ME);
    m_country.Append(AF);
    m_country.Append(MA);

    // 中东：
    country_ME.Append(BL);
    country_ME.Append(SP);
    country_ME.Append(AL);
    country_ME.Append(YS);
    country_ME.Append(YD);
    country_ME.Append(KW);
    country_ME.Append(LB);
    country_ME.Append(ML);
    country_ME.Append(AM);
    country_ME.Append(KT);
    country_ME.Append(ST);
    country_ME.Append(TE);
    country_ME.Append(QZ);
    country_ME.Append(QT_E);

    // 中美洲和南美洲:
    country_MA.Append(AT);
    country_MA.Append(BB);
    country_MA.Append(BM);
    country_MA.Append(KM);
    country_MA.Append(DM);
    country_MA.Append(GL);
    country_MA.Append(YM);
    country_MA.Append(SJ);
    country_MA.Append(SL);
    country_MA.Append(LN);
    country_MA.Append(TL);
    country_MA.Append(QT_A);




    CFileManager    profile;
    string        strMenu, strData, strTemp;
    bool            isbreak = true;
    bool            isshowmessage = false;

    //vector<string>vehicle_name;
    //vector<string>vehicle_year;

//#ifdef    __NT5600DPF__
//    return DiagSoftware();
//#endif
    while (1)
    {
        if (isbreak == false)  // 返回逻辑处理
        {
            MainEntrance();
            break;
        }

        ctrl.InitCtrl(CBinary("\x53\xFF\x00\x03\x00\x00", 6));   // 区域
        for (int i = 0; i < m_country.GetByteCount(); i++)
        {
            ctrl.AddOneItem(m_country[i]);
        }

        iSelected = ctrl.ShowCtrl();     // 国家显示
        if (iSelected == -1)
        {
            if (FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_EXITSYSTEM, DF_MB_YESNO) == DF_IDYES)
                break;
            else
                continue;
        }
        else
        {
            Message_Country = iSelected;    // 选中的项存入车辆信息值中
        }

        if (m_country[iSelected] == JP)   // 根据CBinary的值自定义打开文件类型:   日本/北美/欧洲....................
        {
            Country_Vin_File = SubaruVin_JP_CBF;
        }
        else if (m_country[iSelected] == NA)  // 北美
        {
            Country_Vin_File = SubaruVin_NA_CBF;
        }
        else if (m_country[iSelected] == EU)   // 欧洲
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == ASIA)   // 亚洲/大洋洲
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == AF)   // 非洲
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == ME)  // 中东
        {
        SHOWME:
            Country_Vin_File = SubaruVin_ME_CBF;

            while (1)
            {
                menuME.InitCtrl(CBinary("\x53\xFF\x00\x05\x00\x00", 6)); // 国家

                for (int i = 0; i < country_ME.GetByteCount(); i++)
                {
                    menuME.AddOneItem(country_ME[i]);
                }

                ME_selcet = menuME.ShowCtrl();
                if (ME_selcet == -1)
                {
                    isbreak = false;  // 处理返回逻辑
                    break;
                }
                else
                {
                    Message_ME_MA = ME_selcet;  // 选中的项存入车辆信息值中
                    isbreak = true;   //  != -1 往下走
                    break;
                }
            }
            if (isbreak == false)
            {
                MainEntrance();
                break;
            }
        }
        else if (m_country[iSelected] == MA)  // 中美洲和南美洲
        {
        SHOWMA:
            Country_Vin_File = SubaruVin_CSA_CBF;

            while (1)
            {
                menuMA.InitCtrl(CBinary("\x53\xFF\x00\x05\x00\x00", 6)); // 国家

                for (int i = 0; i < country_MA.GetByteCount(); i++)
                {
                    menuMA.AddOneItem(country_MA[i]);
                }

                MA_select = menuMA.ShowCtrl();
                if (MA_select == -1)
                {
                    country_MA.Clear();   // 清空避免重复
                    isbreak = false;  // 处理返回逻辑
                    break;
                }
                else
                {
                    Message_ME_MA = MA_select;  // 选中的项存入车辆信息值中
                    isbreak = true;
                    break;
                }
            }
            if (isbreak == false)
            {
                MainEntrance();
                break;
            }
        }




        //for (int i = 0;i < binMainMenu.GetByteCount();i++)
        //{
        //    strData = GetStringValue(Country_File,binMainMenu[i]);  // 获取ID
        //    if(!strData.size())
        //        return ecRet = CErrorCode::EC_DATA;
        //    profile.InitManager(strData);  // 截取赋值   SetItemValue
        //    strMenu = profile.GetItemValue(Subaru_VIN_INDEX_TTL,Subaru_VIN_VehicleName_TXT);  
        //    strMenu += Subaru_Vehicle_Name;
        //    binMainMenu[i] = String2Binary(strMenu);
        //    m_vehicle.Append(binMainMenu[i]);
        //}

    SHOWEMPTY:
        while (1)
        {
            // 清空，避免重复
            strData.clear();
            strMenu.clear();
            vehicle_name.clear();
            m_vehicle.Clear();
            m_vehicle_name.Clear();

            strData = GetStringValue(Country_Vin_File, binSearchID);  // 获取ID
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // 截取赋值   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_INDEX_TTL, Subaru_VIN_VehicleIndex_TXT);  // 获取INDEX和VehicleIndex的值

            iNum = SeparatorString(strMenu, ",", vehicle_name);   // String2Binary或者HexString2Binary不能使用string[i]并且只能是单个字符串，用此函数做分离处理
            for (int i = 0; i < iNum; i++)
            {
                binMainMenu = HexString2Binary(vehicle_name[i]);    // string -> CBinary 车型
                m_vehicle.Append(binMainMenu);    // 车型插入CBinarygroup中
            }

            for (int i = 0; i < m_vehicle.GetByteCount(); i++)   // 遍历CBinarygroup
            {
                binMainMenu.Clear();  // 每处理完一项，清空binMainMenu，避免错误
                strData = GetStringValue(Country_Vin_File, m_vehicle[i]);  // 搜索每一项车型的内容
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // 截取赋值   SetItemValue
                strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETTL, Subaru_VIN_VehicleName_TXT);    // 获取车型名称的字符串
                binMainMenu = String2Binary(strMenu);
                Vehcilename += binMainMenu;   // 在字符串前加入\x53\x5D
                m_vehicle_name.Append(Vehcilename);    // 将转换后的车型名称插入新的CBinarygroup
                Vehcilename = CBinary("\x53\x5D", 2);   // 每处理完一项，重置Vehcilename，避免错误
            }



            vehiclemenu.InitCtrl(CBinary("\x53\x60\xFF\x00\x00\x44", 6));
            for (int i = 0; i < m_vehicle_name.GetByteCount(); i++)   // 遍历车型名称
            {
                vehiclemenu.AddOneItem(m_vehicle_name[i]);      // 插入菜单项
            }

            iSelected = vehiclemenu.ShowCtrl();   // 显示车型
            if (iSelected == -1)
            {
                m_vehicle_name.Clear();
                if (Country_Vin_File == SubaruVin_ME_CBF)
                {
                    goto SHOWME;
                }
                else if (Country_Vin_File == SubaruVin_CSA_CBF)
                {
                    goto SHOWMA;
                }

                isbreak = false;   // 处理返回逻辑
                break;
            }
            else
            {
                Message_Vehicle = iSelected;  // 选中的项存入车辆信息值中
            }

            m_vehicle_type.Clear();   // 清空，避免重复

            strData.clear();
            strMenu.clear();
            binMainMenu.Clear();
            strData = GetStringValue(Country_Vin_File, m_vehicle[iSelected]);  // 根据点击选取VehicleIndex
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // 截取赋值   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETTL, Subaru_VIN_VehicleType_TXT);    // 获取车型年份的索引
            //binMainMenu = HexString2Binary(strMenu);
            //m_vehicle_type.Append(binMainMenu);    

            iNum2 = SeparatorString(strMenu, ",", vehicle_year);   // String2Binary或者HexString2Binary不能使用string[i]并且只能是单个字符串，用此函数做分离处理
            for (int i = 0; i < iNum2; i++)
            {
                binMainMenu = HexString2Binary(vehicle_year[i]);    // 车型年份索引
                CBinary temp = CBinary("\x00", 1);
                temp += binMainMenu;
                m_vehicle_type.Append(temp);
            }

            //for (int i = 0;i < binMainMenu.GetByteCount();i++)
            //{
            //    strData = GetStringValue(Country_File,binMainMenu[i]);  // 获取ID
            //    if(!strData.size())
            //        return ecRet = CErrorCode::EC_DATA;
            //    profile.InitManager(strData);  // 截取赋值   SetItemValue
            //    strMenu = profile.GetItemValue(Subaru_VIN_INDEX_TTL,Subaru_VIN_VehicleName_TXT);  
            //    strMenu += Subaru_Vehicle_Name;
            //    binMainMenu[i] = String2Binary(strMenu);
            //    m_vehicle.Append(binMainMenu[i]);
            //}

            m_vehicle_year.Clear();
            for (int i = 0; i < m_vehicle_type.GetByteCount(); i++)
            {

                binMainMenu.Clear();
                strData = GetStringValue(Country_Vin_File, m_vehicle_type[i]);
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // 截取赋值   SetItemValue
                strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_Year_TXT);    // 获取车型年份的内容
                //string TempStr  = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL,"SystemName");    // 获取车型年份的内容
                binMainMenu = String2Binary(strMenu);
                Vehcilename += binMainMenu;
                bool isright = true;
                for (int k = 0; k < m_vehicle_year.GetByteCount(); k++)    // 去除年份一样的菜单项
                {
                    if (m_vehicle_year[k].Compare(Vehcilename) == 0)
                    {
                        isright = false;
                    }
                }

                if (isright && Vehcilename != CBinary("\x53\x5d\x00\x00\x00\x2b", 6) && Vehcilename != CBinary("\x53\x5d", 2))
                {                                                // 如果SystemName为0002b或者为空的话，直接跳入老代码的控制单元，不show年份
                    m_vehicle_year.Append(Vehcilename);
                }
                else if (m_vehicle_year.GetByteCount() == 0)   // SystemName 如果为0002b或为空的话，其他车型肯定不存在,为了避免混乱
                {
                    Vehcilename = CBinary("\x53\x5D", 2);
                    DiagSoftware();  // 老代码显示系统
                    goto SHOWEMPTY;   // 返回车型
                }

                Vehcilename = CBinary("\x53\x5D", 2);
            }

            while (1)
            {
                vehicleyear.InitCtrl(CBinary("\x53\xFF\x00\x02\x00\x00", 6));  // 型号
                for (int i = 0; i < m_vehicle_year.GetByteCount(); i++)   // 遍历车型名称
                {
                    vehicleyear.AddOneItem(m_vehicle_year[i]);      // 插入菜单项
                }

                iSelected = vehicleyear.ShowCtrl();   // 显示车型/年份
                if (iSelected == -1)
                {
                    break;
                }
                else if (iSelected == m_vehicle_year.GetByteCount() - 1)  // 每年份的最后一项是最老年份，所用的是老系统
                {
                    DiagSoftware();  // 老代码显示系统
                    goto SHOWEMPTY;   // 返回车型
                }
                else
                {
                Vechilemessage:
                    Message_Year = iSelected;  // 选中的项存入车辆信息值中

                    if (isshowmessage == true)  // 处理车辆信息的返回状态
                    {
                        CEcuInfoCtrl Menumessage;
                        Menumessage.AddOneItem(CBinary("\x53\xFF\x00\x03\x00\x00", 6), FxGetStdString(m_country[Message_Country]));
                        Menumessage.AddOneItem(CBinary("\x53\x60\xFF\x00\x00\x44", 6), FxGetStdString(m_vehicle_name[Message_Vehicle]));
                        Menumessage.AddOneItem(CBinary("\x53\xFF\x00\x01\x00\x00", 6), FxGetStdString(m_vehicle_year[Message_Year]));
                        //Menumessage.AddOneItem(FxGetStdString(m_country[Message_Country]),FxGetStdString(m_country[Message_Country]));

                        while (1)
                        {
                            Message_select = Menumessage.ShowCtrl();   // 车辆信息
                            if (Message_select == -1)
                            {
                                isshowmessage = false;    // 车辆信息返回状态
                                break;
                            }
                        }
                    }


                    while (1)
                    {
                        mnuMenu.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x35", 6));
                        mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x60", 6));// 诊断
                        mnuMenu.AddOneItem(CBinary("\xFF\x07\x00\x00\x00\x40", 6));// 常用特殊功能
                        mnuMenu.AddOneItem(CBinary("\x01\x00\x00\x00\x01\x1B", 6));// 车辆信息
                        iSelMenuItem = mnuMenu.ShowCtrl();
                        if (iSelMenuItem == -1)
                        {
                            break;
                        }
                        switch (iSelMenuItem)
                        {
                        case 0x00:            // 诊断
                            m_bIsService = false;
                            //DiagSoftware();    
                            Subaru_DiagSoftware();
                            break;
                        case 0x01:            // 常用特殊功能
                            m_bIsService = true;
                            //Service();//
                            HotFunction();
                            m_bIsService = false;
                            break;
                        case 0x02:
                            //m_bIsService = true;
                            //isProgramming = true;
                            //Programming();    // 编程
                            //m_bIsService = false;
                            //isProgramming = false;
                            isshowmessage = true;
                            goto Vechilemessage;   // 车辆信息
                            break;
                        }
                    }
                }
            }

        }
        continue;
    }


    //if (iSelected == 1)   // BRZ车型的检查
    //{
    //    VechileStatus = 2;
    //    ecRet = DiagSoftware_BRZ();
    //}

    isbreak = true;
    return 0;
}

W_ErrorCode CSubaru::ChooseVehicle()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    W_I16            iSelMenuItem, i, k;
    CMenuCtrl        uiMenu, uiMenuNA, uiMenuJP, uiMenuEU, uiMenuCSA, uiMenuVehicele, uiMenuYear;
    CBinary            binTemp;
    string            strShow;
    CSearchString    Suba_ManualOilResetMode, SubaruHotFunMenuNA, SubaruHotFunMenuJP, SubaruHotFunMenuEU, SubaruHotFunMenuCSA, SubareHotFun;
    vector<string>    vctStr, vctcountry, vctStrVehicleNA, vctStrVehicleJP, vctStrVehicleEU, vctStrVehicleCSA, vctStrVehicle;
    vector<string> vctYearNA, vctYearJP, vctYearEU, vctYearCSA, vctYear;
    vector<vector<string>>    SearchResult, SearchResultNA, SearchResultJP, SearchResultEU, SearchResultCSA, SearchStrSubMenu, SearchResultVehicle;
    //vector<vector<string>>    SearchResult,SearchResultNA,SearchResultJP,SearchResultEU,SearchResultCSA ,SearchStrSubMenu;

    if (FALSE == Suba_ManualOilResetMode.OpenTabFile(Suba_ManualOilResetMode_CBF))
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }
    vctcountry.clear();    //车型
    vctStr.clear();
    //vctStr.push_back("49");
    //vctStr.push_back("60");
    //vctStr.push_back("53");
    //vctStr.push_back("44");
    //vctStr.push_back("66");
    //vctStr.push_back("57");
    //vctStr.push_back("58");
    //vctStr.push_back("56");
    //vctStr.push_back("50");
    //vctStr.push_back("47");
    //vctStr.push_back("62");
    //vctStr.push_back("76");
    //vctStr.push_back("67");
    //vctStr.push_back("54");
    //vctStr.push_back("63");
    //vctStr.push_back("77");
    //vctStr.push_back("68");

    vctStr.push_back("NA");
    vctStr.push_back("JP");
    vctStr.push_back("EU");
    vctStr.push_back("CSA");

    for (i = 0; i < 4; i++)
    {
        Suba_ManualOilResetMode.SearchString(SearchResult, FALSE, 1, 1, vctStr);
        vector<string>::iterator it = vctStr.begin();
        vctStr.erase(it);
    }

    for (i = 0; i < SearchResult.size(); i++)
    {
        for (k = 0; k < vctcountry.size(); k++)    //获取常用特殊功能的菜单并去重复处理
        {
            if (SearchResult[i][1] == vctcountry[k])
                break;
        }
        if (k == vctcountry.size())
            vctcountry.push_back(SearchResult[i][1]);
    }

    while (1)
    {
        uiMenu.InitCtrl(CBinary("\x53\xFF\x00\x03\x00\x00", 6));   // 区域
        for (i = 0; i < vctcountry.size(); i++)
        {
            //binTemp = HexString2Binary(vctStrVehicle[i]);
            uiMenu.AddOneItem(vctcountry[i]);
        }
        iSelMenuItem = uiMenu.ShowCtrl();
        if (iSelMenuItem == -1)
        {
            if (FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_EXITSYSTEM, DF_MB_YESNO) == DF_IDYES)
                return 0;
        }
        else if (iSelMenuItem == 0)  // 北美
        {
            vctStr.clear();

            if (FALSE == SubareHotFun.OpenTabFile(SubaruHotFunMenuNA_CBF))
            {
                CErrorCode::SetLastError(CErrorCode::EC_DATA);
                return CErrorCode::EC_DATA;
            }

            vctStrVehicle.clear();
            vctStr.clear();
            vctStr.push_back("64");
            SubareHotFun.SearchString(SearchResultVehicle, FALSE, 0, 0, vctStr);
        }
        else if (iSelMenuItem == 1) // 日本
        {
            vctStr.clear();

            if (FALSE == SubareHotFun.OpenTabFile(SubaruHotFunMenuJP_CBF))
            {
                CErrorCode::SetLastError(CErrorCode::EC_DATA);
                return CErrorCode::EC_DATA;
            }

            vctStrVehicle.clear();
            vctStr.clear();
            vctStr.push_back("64");
            SubareHotFun.SearchString(SearchResultVehicle, FALSE, 0, 0, vctStr);
        }
        else if (iSelMenuItem == 2)  // 欧洲
        {
            vctStr.clear();

            if (FALSE == SubareHotFun.OpenTabFile(SubaruHotFunMenuEU_CBF))
            {
                CErrorCode::SetLastError(CErrorCode::EC_DATA);
                return CErrorCode::EC_DATA;
            }

            vctStrVehicle.clear();
            vctStr.clear();
            vctStr.push_back("64");
            SubareHotFun.SearchString(SearchResultVehicle, FALSE, 0, 0, vctStr);
        }
        else if (iSelMenuItem == 3)  // 亚洲
        {
            vctStr.clear();

            if (FALSE == SubareHotFun.OpenTabFile(SubaruHotFunMenuCSA_CBF))
            {
                CErrorCode::SetLastError(CErrorCode::EC_DATA);
                return CErrorCode::EC_DATA;
            }

            vctStrVehicle.clear();
            vctStr.clear();
            vctStr.push_back("64");
            SubareHotFun.SearchString(SearchResultVehicle, FALSE, 0, 0, vctStr);
        }


        // 车型
        for (i = 0; i < SearchResultVehicle.size(); i++)
        {
            for (k = 0; k < vctStrVehicle.size(); k++)    //获取常用特殊功能的菜单并去重复处理
            {
                if (SearchResultVehicle[i][1] == vctStrVehicle[k])
                    break;
            }
            if (k == vctStrVehicle.size())
                vctStrVehicle.push_back(SearchResultVehicle[i][1]);
        }

        uiMenuVehicele.InitCtrl(CBinary("\x53\x5D\x00\x00\x2F\x1E", 6));
        for (i = 0; i < vctStrVehicle.size(); i++)
        {
            binTemp = HexString2Binary(vctStrVehicle[i]);
            uiMenuVehicele.AddOneItem(binTemp);
        }
        iSelMenuItem = uiMenuVehicele.ShowCtrl();
        if (iSelMenuItem == -1)
        {
            continue;
        }


        // 年份
        vctYear.clear();

        for (int i = 0; i < SearchResultVehicle.size(); i++)
        {
            if (SearchResultVehicle[i][1] == vctStrVehicle[iSelMenuItem])
            {
                for (k = 0; k < vctYear.size(); k++)    //获取常用特殊功能的菜单并去重复处理
                {
                    if (SearchResultVehicle[i][2] == vctYear[k])
                        break;
                }
                if (k == vctYear.size())
                    vctYear.push_back(SearchResultVehicle[i][2]);
            }
        }

        while (1)
        {
            uiMenuYear.InitCtrl(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));
            for (i = 0; i < vctYear.size(); i++)
            {
                binTemp = HexString2Binary(vctYear[i]);
                uiMenuYear.AddOneItem(binTemp);
            }
            iSelMenuItem = uiMenuYear.ShowCtrl();
            if (iSelMenuItem == -1)
            {
                break;
            }
            MainEntrance();
        }


        //else if (iSelMenuItem == 0)  // 北美车型
        //{
        //    if (FALSE == SubaruHotFunMenuNA.OpenTabFile(SubaruHotFunMenuNA_CBF))
        //    {
        //        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        //        return CErrorCode::EC_DATA;
        //    }

        //    vctStrVehicleNA.clear();
        //    vctStr.clear();
        //    vctStr.push_back("64");

        //    SubaruHotFunMenuNA.SearchString(SearchResultNA, FALSE, 0, 0, vctStr);
        //
        //    for (i = 0; i < SearchResultNA.size(); i ++)
        //    {        
        //        for (k = 0; k < vctStrVehicleNA.size(); k ++)    //获取常用特殊功能的菜单并去重复处理
        //        {
        //            if (SearchResultNA[i][1] == vctStrVehicleNA[k])
        //                break;
        //        }
        //        if (k == vctStrVehicleNA.size())
        //            vctStrVehicleNA.push_back(SearchResultNA[i][1]);        
        //    }

        //    uiMenuNA.InitCtrl(CBinary("\x53\x5D\x00\x00\x2F\x1E", 6));
        //    for (i = 0; i < vctStrVehicleNA.size(); i++)
        //    {
        //        binTemp = HexString2Binary(vctStrVehicleNA[i]);
        //        uiMenuNA.AddOneItem(binTemp);
        //    }
        //    iSelMenuItem = uiMenuNA.ShowCtrl();
        //    if (iSelMenuItem == -1)
        //    {
        //        if(FxShowMessageBox(STD_TTL_MSG_INFORMATION,STD_TXT_MSG_EXITSYSTEM,DF_MB_YESNO)==DF_IDYES)
        //            return 0;            
        //    }

        //    // 北美车型年份
        //    vctYearNA.clear();
        //        
        //    for (int i = 0; i < SearchResultNA.size(); i++)
        //    {
        //        if (SearchResultNA[i][1] == vctStrVehicleNA[iSelMenuItem])
        //        {
        //            for (k = 0; k < vctYearNA.size(); k ++)    //获取常用特殊功能的菜单并去重复处理
        //            {
        //                if (SearchResultNA[i][2] == vctYearNA[k])
        //                    break;
        //            }
        //            if (k == vctYearNA.size())
        //                vctYearNA.push_back(SearchResultNA[i][2]);    
        //        }
        //    }
        //    
        //    while (1)
        //    {
        //        uiMenuNA.InitCtrl(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));
        //        for (i = 0; i < vctYearNA.size(); i++)
        //        {
        //            binTemp = HexString2Binary(vctYearNA[i]);
        //            uiMenuNA.AddOneItem(binTemp);
        //        }
        //        iSelMenuItem = uiMenuNA.ShowCtrl();
        //        if (iSelMenuItem == -1)
        //        {
        //            break;
        //        }
        //    }
        //}
        //else if (iSelMenuItem == 1)  // 日本车型
        //{
        //    





        //}
        //else if (iSelMenuItem == 2)
        //{

        //}
        //else if (iSelMenuItem == 3)
        //{

        //}

        //MainEntrance();
    }









    //vctStrYearMenu.clear();   // 年份
    //SearchStrSubMenu.clear();
    //for (i = 0; i < SearchResult.size(); i ++)
    //{        
    //    if (SearchResult[i][1] == vctStrVehicleMenu[iSelMenuItem])
    //    {
    //        SearchStrSubMenu.push_back(SearchResult[i]);
    //        for (k = 0; k < vctStrYearMenu.size(); k ++)    //获取常用特殊功能的菜单并去重复处理
    //        {
    //            if (SearchResult[i][2] == vctStrYearMenu[k])
    //                break;
    //        }
    //        if (k == vctStrYearMenu.size())
    //            vctStrYearMenu.push_back(SearchResult[i][2]);    
    //    }
    //}

    //CMenuCtrl    uiYearMenu;
    //while (1)
    //{
    //    uiYearMenu.InitCtrl(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));
    //    for (i = 0; i < vctStrYearMenu.size(); i++)
    //    {
    //        binTemp = HexString2Binary(vctStrYearMenu[i]);
    //        uiYearMenu.AddOneItem(binTemp);
    //    }
    //    iSelMenuItem = uiYearMenu.ShowCtrl();
    //    if (iSelMenuItem == -1)
    //    {
    //        break;
    //    }

        //MainEntrance();
        //for (i = 0; i < SearchStrSubMenu.size(); i ++)
        //{
        //    if (SearchStrSubMenu[i][2] == vctStrYearMenu[iSelMenuItem])
        //    {
        //        strShow = SearchStrSubMenu[i][7];
        //        break;
        //    }
        //}

        //binTemp = HexString2Binary(strShow);
        //FxShowMessageBox(CBinary("\xFF\x05\x00\x00\x00\x6C", 6), binTemp, DF_MB_OK, DT_LEFT);
//    }
//}

////return ecRet;

}//
W_ErrorCode CSubaru::ActiveTest_d()
{
    W_I16 iRet;
    return iRet;
}

W_ErrorCode CSubaru::Subaru_DiagSoftware()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    CMenuCtrl        mnuMenu, Menu_system_name, Menu_Act, Menu_Dtc;
    CEcuInfoCtrl menudtc;
    CVehicleComm    EcuInterface;
    CBinary            binSearchID;
    CBinary            binSubSearchId(Subaru_ID"\x00\x00\x00\x00\x00", 6);//Kas
    CBinary            binMainMenu;
    CBinary            binTemp;
    CFileManager    profile;
    CSearchString        SubaDtc, SubaAct;
    CVehicleSave    VehicleSave;//保存的车型
    string            strMenu, strData, strTemp;
    //W_I16            iSelMenuItem = 0;
    W_I16            iNum = 0;
    Vehcilename = CBinary("\x53\x5D", 2);

    vector<string>vsystem_mod;
    vector<vector<string>> SearchReasault;
    string Act_id;

    // ECU
    if (Country_Vin_File == SubaruVin_JP_CBF)
    {
        Country_Ecu_File = SubaruECU_JP_CBF;
    }
    else if (Country_Vin_File == SubaruVin_NA_CBF)
    {
        Country_Ecu_File = SubaruECU_NA_CBF;
    }
    else if (Country_Vin_File == SubaruVin_EU_CBF)
    {
        Country_Ecu_File = SubaruECU_EU_CBF;
    }
    else if (Country_Vin_File == SubaruVin_CSA_CBF)
    {
        Country_Ecu_File = SubaruECU_CSA_CBF;
    }
    else if (Country_Vin_File == SubaruVin_ME_CBF)
    {
        Country_Ecu_File = SubaruECU_ME_CBF;
    }


    //m_vehicle_name.Clear();
    while (1)
    {
        mnuMenu.AddOneItem(CBinary("\x01\x00\x00\x00\x01\x0D", 6));  // 自动扫描
        mnuMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x00\x02", 6)); // 控制单元
        iSelMenuItem = mnuMenu.ShowCtrl();
        if (iSelMenuItem == -1)
        {
            break;
        }
        else
        {
            vechile_system_name.clear();
            m_system_name.Clear();
            strData = GetStringValue(Country_Vin_File, m_vehicle_type[iSelected]);  // 搜索每一项车型的内容
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // 截取赋值   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_SystemName_TXT);  // 获取控制系统名称

            iNum = SeparatorString(strMenu, ",", vechile_system_name);
            for (int i = 0; i < iNum; i++)
            {
                binMainMenu = HexString2Binary(vechile_system_name[i]);
                Vehcilename += binMainMenu;
                m_system_name.Append(Vehcilename);
                Vehcilename = CBinary("\x53\x5D", 2);
            }


            for (int i = 0; i < m_system_name.GetByteCount(); i++)
            {
                Menu_system_name.AddOneItem(m_system_name[i]);
            }
            iSelMenuItem = Menu_system_name.ShowCtrl();    // 显示系统名字
            if (iSelMenuItem == -1)
            {
                return 0;
            }
        }


        strData.clear();
        strMenu.clear();
        binMainMenu.Clear();
        strData = GetStringValue(Country_Vin_File, m_vehicle_type[iSelected]);
        if (!strData.size())
            return ecRet = CErrorCode::EC_DATA;
        profile.InitManager(strData);  // 截取赋值   SetItemValue
        strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_ECUID);

        SeparatorString(strMenu, ",", Vin_Ecu_Id);

        SeparatorString(Vin_Ecu_Id[iSelMenuItem], "&", Vin_Ecu_Id_Select);   // 根据点击项对应的ECUID组

        //Find_New_SeparatorString(strMenu,",","&",Vin_Ecu_Id);  // 拆分ECUID  分离","  分离"&"

        for (int i = 0; i < Vin_Ecu_Id_Select.size(); i++)
        {
            if (i == 0)    // 取每组的第一个
            {
                System_Search_Id = Vin_Ecu_Id_Select[i];  // 用于搜索故障码的编号，string类型
                binTemp = HexString2Binary(Vin_Ecu_Id_Select[i]);
                CBinary bintempadd = CBinary("\x00\x00", 2);
                bintempadd += binTemp;
                Ecu_Id_Select.Append(bintempadd);
                for (int j = 0; j < Ecu_Id_Select.GetByteCount(); j++)
                {
                    binSearchID = Ecu_Id_Select[i];
                }
            }
        }

        if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
        {
            ecRet = New_SubaCANProInto(binSearchID);  // 发命令

            if (NULL != m_pDiagnosisAppLayer)
            {
                delete m_pDiagnosisAppLayer;
                m_pDiagnosisAppLayer = NULL;
            }
        }
        else
        {
            m_vcttoken.clear();
            m_vctkastoken.clear();
            if (CErrorCode::EC_SUCCESS != QuickTestProcess())
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
            }
        }




        //for (int i = 0; i < Ecu_Id_Select.GetByteCount();i++)
        //{
        //    if (FALSE == SubaAct.OpenTabFile(Suba_DTC_Dex_CBF))  //故障码命令和方式
        //        return CErrorCode::EC_ECU_REFUSE;

        //    vsystem_mod.push_back("00000ac7");
        //    SubaDtc.SearchString(SearchReasault, FALSE, 1, 1, vsystem_mod);   


        //}

        //strData = GetStringValue(Country_Ecu_File,Ecu_Id_All[i]);
        //if(!strData.size())
        //    return ecRet = CErrorCode::EC_DATA;
        //    break;
        //profile.InitManager(strData);  // 截取赋值   SetItemValue
        //strMenu = profile.GetItemValue(Subaru_DTC_TTL,Subaru_BytePosDtc_TXT);  
        //if (strMenu == "1")
        //{
        //    Ecu_Id_Select.Append(Ecu_Id_All[i]);
        //    temp.push_back(strMenu);
        //}



            //if (Vin_Ecu_Id[i] == "00000ac7")   // TCM变速器
            //{
            //    if (FALSE == SubaAct.OpenTabFile(Suba_DTC_Dex_CBF))  //故障码命令和方式
            //        return CErrorCode::EC_ECU_REFUSE;

            //    vsystem_mod.push_back("00000ac7");
            //    SubaAct.SearchString(SearchReasault, FALSE, 1, 1, vsystem_mod);   

            //    for (int j = 0; j < SearchReasault.size(); j++)
            //    {
            //        for (int k = 0; k < SearchReasault.size(); k++)
            //        {
            //            // 建立临时变量接收拼接值
            //            string acttemp = SearchReasault[j][k+1] + SearchReasault[j][k+2] + SearchReasault[j][k+3];   // 拼接Suba_DTC_Dex [1] [2] [3]
            //            CBinary bintemp = HexString2Binary(acttemp);
            //            Dtc_Ecu_Id.Append(bintemp);
            //            break;
            //        }
            //    }
            //}



        //for(int i = 0; i < Dtc_Ecu_Id.GetByteCount(); i++)
        //{
        //    strData = GetStringValue(Suba_DTC_NEW_CBF,Dtc_Ecu_Id[i]);  
        //    if(!strData.size())
        //        return ecRet = CErrorCode::EC_DATA;
        //    profile.InitManager(strData);  // 截取赋值   SetItemValue
        //    strMenu = profile.GetItemValue(Subaru_DTC_TTL,Subaru_DtcText_TXT);  
        //    CBinary menubin = HexString2Binary(strMenu);
        //    Vehcilename = CBinary("\x53\x5D",2);
        //    Vehcilename += menubin;
        //    

        //    Menu_Dtc.AddOneItem(Vehcilename);
        //}
        //Menu_Dtc.ShowCtrl();




        Ecu_Id_Select.Clear();  // 清空，命令更新
        strMenu.clear();
        Vin_Ecu_Id.clear();
        SearchReasault.clear();
        vsystem_mod.clear();
        Dtc_Ecu_Id.Clear();
    }










    VechileStatus = 1;  // 其他车型
    strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);  // 获取ID
    if (!strData.size())
        return ecRet = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // 截取赋值   SetItemValue
    strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);  // 获取Menu和MenuItem的值
    binMainMenu = String2Binary(strMenu);
    if (VehicleSave.GetCount())
        binMainMenu += VehicleRecord;
    while (1)
    {
        iSelMenuItem = 1;
#if defined(__NT800__) || defined(__NT5600ALLSYS__)
        iSelMenuItem = FxShowMenu(Subaru_TTL_MNU_MAIN, binMainMenu, mnuMenu);    //自动扫描 和 按系统测试 菜单
#endif
        if (-1 == iSelMenuItem)
        {
            break;
        }
        if (iSelMenuItem >= 0)
        {
            m_ucSelectItem = binMainMenu[iSelMenuItem];  // MenItem 1,2
            if (m_ucSelectItem == System_Unit)    //按系统测试 菜单
            {
                binSearchID.SetByteAt(4, m_ucSelectItem);  // 0x53,0x00,0x00,0x00,0x02,0x00    
                binSubSearchId.SetByteAt(4, m_ucSelectItem);//Kas
                strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // MenuItem的值
                strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
                CBinary binSubMenu;
                CMenuCtrl mUnitMenu;
                CMenuCtrl    mMenuSub;
                CMenuCtrl    mMenu;
                //CBinary binChild;
                binSubMenu = String2Binary(strMenu);
                binSubMenu += CBinary("\xff", 1);   //添加 OBD系统 菜单

#ifdef    __NT5600DEF__
                CBinary binMenuTemp;
#ifdef    __NT5600FOUR__
                binMenuTemp += ControlModel_ENG;
                binMenuTemp += ControlModel_AT;
                binMenuTemp += ControlModel_ABS;
                binMenuTemp += ControlModel_SRS;
#endif
#ifdef    __NT5600AA__
                binMenuTemp += ControlModel_ABS;
                binMenuTemp += ControlModel_SRS;
#endif
#ifdef    __NT5600SAS__
                binMenuTemp += ControlModel_EPS;
#endif
#ifdef    __NT5600DPF__
                binMenuTemp += ControlModel_ENG;
#endif
#ifdef    __NT5600ALLSYS__
                binMenuTemp = binSubMenu;
#endif
                binSubMenu.Clear();
                binSubMenu = binMenuTemp;
                binMenuTemp.Clear();
#endif
                while (1)
                {
                    //iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN,binSubMenu,mMenu);
                    W_I16 iSecSelItem = 0;
                    if (1 < binSubMenu.GetByteCount())
                        iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN, binSubMenu, mUnitMenu);  // 控制单元菜单项显示
                    if (-1 == iSecSelItem)
                        break;

                    /* 先尝试CAN */
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                    {
                        CBinary PtlCan;
                        binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);  // // 0x53,0x00,0x00,0x00,0x00,0x00    
                        binTemp.SetByteAt(5, binSubMenu[iSecSelItem]);   // （5，控制单元MenuItem[点击位置]）
                        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                        if (strTemp.length() > 0)
                        {
                            profile.InitManager(strTemp);
                            PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);  // 取对应的ItemCan协议
                        }
                        if (PtlCan.GetByteCount() > 0)
                        {
                            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                            binTemp = Subaru_CANID_BASE;   //  0x53,0x4d,0x00,0x00,0x00,0x00    
                            binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);  // （5，取对应的ItemCan的最后一项）

                            ecRet = SubaCANProInto(binTemp);  // 发命令
                        }
                        else
                        {
                            ecRet = CErrorCode::EC_COMMUNICATION;
                        }
                        if (NULL != m_pDiagnosisAppLayer)
                        {
                            delete m_pDiagnosisAppLayer;
                            m_pDiagnosisAppLayer = NULL;
                        }
                        if (CErrorCode::EC_SUCCESS == ecRet)
                        {
                            if (1 == binSubMenu.GetByteCount())
                                break;
                            else
                                continue;
                        }
                    }

                    if (binSubMenu[iSecSelItem] == 0x09)//Keyless access system
                    {
                        W_I16 iNum;
                        W_I16 iSelMenu;
                        //CMenuCtrl    mMenu;
                        CBinary binMenu;
                        CBinary binChild;
                        string strSubMenu;
                        vector<string> vectMenu;
                        CBinary binTitle(Subaru_ID"\x00\x00\x00\x17\x00", 6);
                        CBinary binSubTitle(Subaru_ID"\x00\x00\x00\x18\x00", 6);
                        binSubSearchId.SetByteAt(5, 0x09);
                        strData = GetStringValue(Subaru_Protocol_Ini_File, binSubSearchId);
                        if (!strData.size())
                            return ecRet = CErrorCode::EC_DATA;
                        profile.InitManager(strData);
                        strSubMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_Child_TXT);
                        iNum = SeparatorString(strSubMenu, ",", vectMenu);
                        for (W_I16 i = 0; i < iNum; i++)
                        {
                            binChild = HexString2Binary(vectMenu[i]);
                            binMenu += binChild[2];
                        }
                        while (1)
                        {
                            //CMenuCtrl mMenuSub;
                            iSelMenu = FxShowMenu(binTitle, binMenu, mMenu);
                            if (-1 == iSelMenu)
                                break;
                            m_ucKasSelItem = binMenu[iSelMenu];
                            vector<string> vectSubMenu;
                            CBinary binSSubMenu;
                            strSubMenu = profile.GetItemValue(vectMenu[iSelMenu], Subaru_INI_MenuItem_TXT);
                            if (strSubMenu.size())
                            {
                                iNum = SeparatorString(strSubMenu, ",", vectSubMenu);
                                for (W_I16 i = 0; i < iNum; i++)
                                    binSSubMenu += String2Binary(vectSubMenu[i]);
                            }
                            while (1)
                            {
                                if (strSubMenu.size())
                                {
                                    iSelMenu = FxShowMenu(binSubTitle, binSSubMenu, mMenuSub);
                                    if (-1 == iSelMenu)
                                        break;
                                }
                                m_ucSecSelItem = binSubMenu[iSecSelItem];//KAS进入命令都相同

                                // xxm 2016-08-11 根据实测修改
                                W_I16 iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                W_I16 SysNumTemp = 0;
                                while (CErrorCode::EC_SUCCESS == iRet)
                                {
                                    if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                    {
                                        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                        break;
                                    }
                                    SysNumTemp = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum + 1;

                                    W_I8    iSysNum = 0;
                                    CBinary bin;

                                    m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                    for (W_I16 i = 0; i < m_bgSysId.GetByteCount(); i++)
                                    {
                                        iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                    }
                                    m_binSysID.Clear();
                                    if (iSysNum == 1)
                                    {
                                        for (int k = 0; k < 5; k++)
                                            bin += m_bgSysId[0][k];
                                        m_binSysID = bin;
                                        if (bin[0] >= 0x40 && bin[0] < 0x80)
                                        {

                                        }
                                        else if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                        {
                                            if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                            {
                                                iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                                ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum = SysNumTemp;
                                            }
                                            else
                                                break;
                                        }
                                    }
                                }
                                if (!strSubMenu.size())//如果此项循环内无子菜单，则退出循环
                                    break;
                            }
                        }
                    }
                    else if (binSubMenu[iSecSelItem] == 0xff)//OBD系统
                    {
                        W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
                        CEobdAllSystem    EobdAllSystem;
                        W_U8            i;

                        for (i = CEobdAllSystem::PROTOCOL_BEGIN + 1; i < CEobdAllSystem::PROTOCOL_KWP; i++) //OBD系统只测CAN协议
                        {
                            //ShowCommWaitInfo();
                            CBinary        binCan("\x01\x00\x00\x00\x00\x00", 6);
                            binCan.SetByteAt(5, (2 + i));
                            string   strTtl = FxGetStdString(CBinary(Subaru_ID"\x00\x00\x00\x01\xFF", 6));
                            string     strCan = FxGetStdString(binCan);

                            FxShowMessageBox(strTtl, strCan, DF_MB_NOBUTTON, DT_CENTER);

                            int itemp = EobdAllSystem.SetMode(i);
                            CErrorCode::Clear();
                            if (CErrorCode::EC_SUCCESS == (ecRet = EobdAllSystem.Entrance(ALM_DIAGNOSE)))
                                break;
                        }
                    }
                    else//非Keyless access system项
                    {
                        m_ucSecSelItem = binSubMenu[iSecSelItem];
                        if (iSecSelItem >= 0)
                        {
                            // xxm 2016-08-11 根据实测修改
                            W_I16 iRet = NewAppLayer(Subaru_SEARCH_SYS);
                            W_I16 SysNumTemp = 0;
                            while (CErrorCode::EC_SUCCESS == iRet)
                            {
                                if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                {
                                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                    break;
                                }
                                SysNumTemp = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum + 1;
                                W_I8    iSysNum = 0;
                                CBinary bin;

                                m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                for (W_I16 i = 0; i < m_bgSysId.GetByteCount(); i++)
                                {
                                    iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                }
                                m_binSysID.Clear();
                                if (iSysNum == 1)
                                {
                                    for (int k = 0; k < 5; k++)
                                        bin += m_bgSysId[0][k];
                                    m_binSysID = bin;
                                    if (bin[0] >= 0x40 && bin[0] < 0x80)
                                    {

                                    }
                                    else if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                    {
                                        if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                        {
                                            // 如果这个协议没有进入成功,继续扫描下一个协议,直到SysNumTemp大于等于可扫描的协议数或进入成功
                                            iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                            ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum = SysNumTemp;
                                        }
                                        else
                                            break;
                                    }
                                }
                            }
                        }
                    }
                    if (1 == binSubMenu.GetByteCount())
                        break;
                }
            }
            else if (m_ucSelectItem == VehicleRecord)  // 自动扫描
            {
                if (CErrorCode::EC_SUCCESS != SubaruVehicleRecord())
                {
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                }
            }
            else
            {
                m_vcttoken.clear();
                m_vctkastoken.clear();
                if (CErrorCode::EC_SUCCESS != QuickTestProcess())
                {
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                }
            }
        }
#ifndef    __NT800__
#ifndef __NT5600ALLSYS__
        break;
#endif
#endif
    }

    return ecRet;
}

W_ErrorCode CSubaru::DiagSoftware()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    CMenuCtrl        mnuMenu;
    CVehicleComm    EcuInterface;
    CBinary            binSearchID(Subaru_ID"\x00\x00\x00\x00\x00", 6);
    CBinary            binSubSearchId(Subaru_ID"\x00\x00\x00\x00\x00", 6);//Kas
    CBinary            binMainMenu;
    CBinary            binTemp;
    CFileManager    profile;
    CVehicleSave    VehicleSave;//保存的车型
    string            strMenu, strData, strTemp;
    W_I16            iSelMenuItem = 0;

    VechileStatus = 1;  // 其他车型
    strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);  // 获取ID
    if (!strData.size())
        return ecRet = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // 截取赋值   SetItemValue
    strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);  // 获取Menu和MenuItem的值
    binMainMenu = String2Binary(strMenu);
    if (VehicleSave.GetCount())
        binMainMenu += VehicleRecord;
    while (1)
    {
        iSelMenuItem = 1;
#if defined(__NT800__) || defined(__NT5600ALLSYS__)
        iSelMenuItem = FxShowMenu(Subaru_TTL_MNU_MAIN, binMainMenu, mnuMenu);    //自动扫描 和 按系统测试 菜单
#endif
        if (-1 == iSelMenuItem)
        {
            break;
        }
        if (iSelMenuItem >= 0)
        {
            m_ucSelectItem = binMainMenu[iSelMenuItem];  // MenItem 1,2
            if (m_ucSelectItem == System_Unit)    //按系统测试 菜单
            {
                binSearchID.SetByteAt(4, m_ucSelectItem);  // 0x53,0x00,0x00,0x00,0x02,0x00    
                binSubSearchId.SetByteAt(4, m_ucSelectItem);//Kas
                strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // MenuItem的值
                strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
                CBinary binSubMenu;
                CMenuCtrl mUnitMenu;
                CMenuCtrl    mMenuSub;
                CMenuCtrl    mMenu;
                //CBinary binChild;
                binSubMenu = String2Binary(strMenu);
                binSubMenu += CBinary("\xff", 1);   //添加 OBD系统 菜单

#ifdef    __NT5600DEF__
                CBinary binMenuTemp;
#ifdef    __NT5600FOUR__
                binMenuTemp += ControlModel_ENG;
                binMenuTemp += ControlModel_AT;
                binMenuTemp += ControlModel_ABS;
                binMenuTemp += ControlModel_SRS;
#endif
#ifdef    __NT5600AA__
                binMenuTemp += ControlModel_ABS;
                binMenuTemp += ControlModel_SRS;
#endif
#ifdef    __NT5600SAS__
                binMenuTemp += ControlModel_EPS;
#endif
#ifdef    __NT5600DPF__
                binMenuTemp += ControlModel_ENG;
#endif
#ifdef    __NT5600ALLSYS__
                binMenuTemp = binSubMenu;
#endif
                binSubMenu.Clear();
                binSubMenu = binMenuTemp;
                binMenuTemp.Clear();
#endif
                while (1)
                {
                    //iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN,binSubMenu,mMenu);
                    W_I16 iSecSelItem = 0;
                    if (1 < binSubMenu.GetByteCount())
                        iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN, binSubMenu, mUnitMenu);  // 控制单元菜单项显示
                    if (-1 == iSecSelItem)
                        break;

                    /* 先尝试CAN */
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                    {
                        CBinary PtlCan;
                        binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);  // // 0x53,0x00,0x00,0x00,0x00,0x00    
                        binTemp.SetByteAt(5, binSubMenu[iSecSelItem]);   // （5，控制单元MenuItem[点击位置]）
                        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                        if (strTemp.length() > 0)
                        {
                            profile.InitManager(strTemp);
                            PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);  // 取对应的ItemCan协议
                        }
                        if (PtlCan.GetByteCount() > 0)
                        {
                            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                            binTemp = Subaru_CANID_BASE;   //  0x53,0x4d,0x00,0x00,0x00,0x00    
                            binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);  // （5，取对应的ItemCan的最后一项）

                            ecRet = SubaCANProInto(binTemp);  // 发命令
                        }
                        else
                        {
                            ecRet = CErrorCode::EC_COMMUNICATION;
                        }
                        if (NULL != m_pDiagnosisAppLayer)
                        {
                            delete m_pDiagnosisAppLayer;
                            m_pDiagnosisAppLayer = NULL;
                        }
                        if (CErrorCode::EC_SUCCESS == ecRet)
                        {
                            if (1 == binSubMenu.GetByteCount())
                                break;
                            else
                                continue;
                        }
                    }

                    if (binSubMenu[iSecSelItem] == 0x09)//Keyless access system
                    {
                        W_I16 iNum;
                        W_I16 iSelMenu;
                        //CMenuCtrl    mMenu;
                        CBinary binMenu;
                        CBinary binChild;
                        string strSubMenu;
                        vector<string> vectMenu;
                        CBinary binTitle(Subaru_ID"\x00\x00\x00\x17\x00", 6);
                        CBinary binSubTitle(Subaru_ID"\x00\x00\x00\x18\x00", 6);
                        binSubSearchId.SetByteAt(5, 0x09);
                        strData = GetStringValue(Subaru_Protocol_Ini_File, binSubSearchId);
                        if (!strData.size())
                            return ecRet = CErrorCode::EC_DATA;
                        profile.InitManager(strData);
                        strSubMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_Child_TXT);
                        iNum = SeparatorString(strSubMenu, ",", vectMenu);
                        for (W_I16 i = 0; i < iNum; i++)
                        {
                            binChild = HexString2Binary(vectMenu[i]);
                            binMenu += binChild[2];
                        }
                        while (1)
                        {
                            //CMenuCtrl mMenuSub;
                            iSelMenu = FxShowMenu(binTitle, binMenu, mMenu);
                            if (-1 == iSelMenu)
                                break;
                            m_ucKasSelItem = binMenu[iSelMenu];
                            vector<string> vectSubMenu;
                            CBinary binSSubMenu;
                            strSubMenu = profile.GetItemValue(vectMenu[iSelMenu], Subaru_INI_MenuItem_TXT);
                            if (strSubMenu.size())
                            {
                                iNum = SeparatorString(strSubMenu, ",", vectSubMenu);
                                for (W_I16 i = 0; i < iNum; i++)
                                    binSSubMenu += String2Binary(vectSubMenu[i]);
                            }
                            while (1)
                            {
                                if (strSubMenu.size())
                                {
                                    iSelMenu = FxShowMenu(binSubTitle, binSSubMenu, mMenuSub);
                                    if (-1 == iSelMenu)
                                        break;
                                }
                                m_ucSecSelItem = binSubMenu[iSecSelItem];//KAS进入命令都相同

                                // xxm 2016-08-11 根据实测修改
                                W_I16 iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                W_I16 SysNumTemp = 0;
                                while (CErrorCode::EC_SUCCESS == iRet)
                                {
                                    if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                    {
                                        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                        break;
                                    }
                                    SysNumTemp = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum + 1;

                                    W_I8    iSysNum = 0;
                                    CBinary bin;

                                    m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                    for (W_I16 i = 0; i < m_bgSysId.GetByteCount(); i++)
                                    {
                                        iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                    }
                                    m_binSysID.Clear();
                                    if (iSysNum == 1)
                                    {
                                        for (int k = 0; k < 5; k++)
                                            bin += m_bgSysId[0][k];
                                        m_binSysID = bin;
                                        if (bin[0] >= 0x40 && bin[0] < 0x80)
                                        {

                                        }
                                        else if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                        {
                                            if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                            {
                                                iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                                ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum = SysNumTemp;
                                            }
                                            else
                                                break;
                                        }
                                    }
                                }
                                if (!strSubMenu.size())//如果此项循环内无子菜单，则退出循环
                                    break;
                            }
                        }
                    }
                    else if (binSubMenu[iSecSelItem] == 0xff)//OBD系统
                    {
                        W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
                        CEobdAllSystem    EobdAllSystem;
                        W_U8            i;

                        for (i = CEobdAllSystem::PROTOCOL_BEGIN + 1; i < CEobdAllSystem::PROTOCOL_KWP; i++) //OBD系统只测CAN协议
                        {
                            //ShowCommWaitInfo();
                            CBinary        binCan("\x01\x00\x00\x00\x00\x00", 6);
                            binCan.SetByteAt(5, (2 + i));
                            string   strTtl = FxGetStdString(CBinary(Subaru_ID"\x00\x00\x00\x01\xFF", 6));
                            string     strCan = FxGetStdString(binCan);

                            FxShowMessageBox(strTtl, strCan, DF_MB_NOBUTTON, DT_CENTER);

                            int itemp = EobdAllSystem.SetMode(i);
                            CErrorCode::Clear();
                            if (CErrorCode::EC_SUCCESS == (ecRet = EobdAllSystem.Entrance(ALM_DIAGNOSE)))
                                break;
                        }
                    }
                    else//非Keyless access system项
                    {
                        m_ucSecSelItem = binSubMenu[iSecSelItem];
                        if (iSecSelItem >= 0)
                        {
                            // xxm 2016-08-11 根据实测修改
                            W_I16 iRet = NewAppLayer(Subaru_SEARCH_SYS);
                            W_I16 SysNumTemp = 0;
                            while (CErrorCode::EC_SUCCESS == iRet)
                            {
                                if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                {
                                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                    break;
                                }
                                SysNumTemp = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum + 1;
                                W_I8    iSysNum = 0;
                                CBinary bin;

                                m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                for (W_I16 i = 0; i < m_bgSysId.GetByteCount(); i++)
                                {
                                    iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                }
                                m_binSysID.Clear();
                                if (iSysNum == 1)
                                {
                                    for (int k = 0; k < 5; k++)
                                        bin += m_bgSysId[0][k];
                                    m_binSysID = bin;
                                    if (bin[0] >= 0x40 && bin[0] < 0x80)
                                    {

                                    }
                                    else if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                    {
                                        if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                        {
                                            // 如果这个协议没有进入成功,继续扫描下一个协议,直到SysNumTemp大于等于可扫描的协议数或进入成功
                                            iRet = NewAppLayer(Subaru_SEARCH_SYS);
                                            ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->nSysNum = SysNumTemp;
                                        }
                                        else
                                            break;
                                    }
                                }
                            }
                        }
                    }
                    if (1 == binSubMenu.GetByteCount())
                        break;
                }
            }
            else if (m_ucSelectItem == VehicleRecord)  // 自动扫描
            {
                if (CErrorCode::EC_SUCCESS != SubaruVehicleRecord())
                {
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                }
            }
            else
            {
                m_vcttoken.clear();
                m_vctkastoken.clear();
                if (CErrorCode::EC_SUCCESS != QuickTestProcess())
                {
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                }
            }
        }
#ifndef    __NT800__
#ifndef __NT5600ALLSYS__
        break;
#endif
#endif
    }

    return ecRet;
}

// DYN
W_ErrorCode CSubaru::New_SubaCANProInto(CBinary binSearchID)
{
    W_ErrorCode rect = CErrorCode::EC_DATA;

    string strTemp, strBaudRate;
    CBinary binTemp, binEcuId, binToolId, binEnterCmd;
    CBinaryGroup binID;
    CSearchString Search_NextID;
    CSearchString Search_SubaDLLPrefix;
    CFileManager profile;
    vector<string> vctTemp;
    vector<vector<string>> vctDLLprefix;
    vector<vector<string>> vct_Resault;

    strTemp = GetStringValue(Country_Ecu_File, binSearchID);
    if (strTemp.length() <= 0)
        return CErrorCode::EC_DATA;
    profile.InitManager(strTemp);  // 截取字符
    strTemp = profile.GetItemValue(Subaru_ECU_TTL, Subaru_ECUAddr_TXT);    // 获取ECUID
    binEcuId = HexString2Binary(strTemp);
    strTemp = profile.GetItemValue(Subaru_ECU_TTL, Subaru_ToolAddr_TXT);    // 获取TOOLID
    binToolId = HexString2Binary(strTemp);
    strTemp = profile.GetItemValue(Subaru_CMD_TTL, Subaru_EnterCmd_TXT);    // 获取ENTERCMD
    binEnterCmd = HexString2Binary(strTemp);

    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID = binToolId;
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ToolID = binEcuId;
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EnterCMD = binEnterCmd;

    vector<string> vctStr;
    strBaudRate = "500000";
    vctStr.push_back("0");
    vctStr.push_back("300");
    vctStr.push_back("1");
    vctStr.push_back("0");
    vctStr.push_back("500");
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = atoi(strBaudRate.c_str());   // 波特率
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = "6,14";   // 引脚
    for (int i = 0; i < vctStr.size(); i++)
    {
        ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));  // 时间参数
    }

    rect = m_pDiagnosisAppLayer->New_Entrance(ALM_DIAGNOSE);
    if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag && CErrorCode::EC_SUCCESS == rect)
        return CErrorCode::EC_SUCCESS;
    else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag)
        return  rect;

    //vector<string> vctStr;
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = profile.GetInt(Subaru_INI_Netlayer_TTL, Subaru_INI_BaudRate_TXT); // 取Netlayer和BaudRate的值  网络层节点和波特率
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ComPin_TXT);  // 取Netlayer和ComPin的值  网络层节点和通讯引脚
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_NextID_TXT);  //  取Netlayer和NextID的值
    ////strTemp = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_CommTime_TXT);  // 取Netlayer和CommTime的值  网络层节点和时间参数
    //SeparatorString(strTemp, ",", vctStr);
    //for (W_I16 i = 0; i < vctStr.size(); i++)
    //    ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));

    //if (1 != VechileStatus && 2 != VechileStatus)
    //    return CErrorCode::EC_DATA;
    //if (FALSE == Search_NextID.OpenTabFile(Subaru_CANProInto_CBF))  //NEXTID查询文件->设备ID,ECU ID,进入命令
    //    return CErrorCode::EC_DATA;
    //if (FALSE == Search_SubaDLLPrefix.OpenTabFile(Subaru_DLLPrefix_CBF))  //系统编号查询
    //    return CErrorCode::EC_DATA;

    //if (1 == VechileStatus)  // 其他车型1,BRZ车型2
    //    strTemp = "00000001";
    //else
    //    strTemp = "00000002";
    //vctTemp.push_back(strTemp);
    //Search_SubaDLLPrefix.SearchString(vctDLLprefix, FALSE, 0, 0, vctTemp);  // 搜索系统编号
    //if (0 >= vctDLLprefix.size())
    //    return CErrorCode::EC_DATA;

    //vctTemp.clear();
    //vctTemp.push_back(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID);  // NextID
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->VehicleStatus = VechileStatus;  // 车型
    //Search_NextID.SearchString(vct_Resault, FALSE, 0, 0, vctTemp);   // SubaCANProInto 中搜索NextID插入 vct_Resault中
    //if (0 >= vct_Resault.size())
    //    return CErrorCode::EC_DATA;
    //for (W_I16 i = 0; i < vct_Resault.size(); i++)  // 遍历
    //{
    //    vctTemp = vct_Resault[i];
    //    if (vctTemp.size() != 7)
    //        continue;

    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID = String2Binary(vctTemp[5]);  // EcuID 为在 SubaCANProInto 搜索的 NextID 的第5列
    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ToolID = String2Binary(vctTemp[4]); // ToolID 为在 SubaCANProInto搜索的 NextID 的第4列
    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EnterCMD = String2Binary(vctTemp[6]);// EnterCMD 为在 SubaCANProInto 搜索的 NextID 的第6列
    //    strTemp = vctTemp[3];  // strTemp 为在 SubaCANProInto 搜索的 NextID 的第3列
    //    for (W_I16 j = 0; j < vctDLLprefix.size(); j++)
    //    {
    //        W_I16 IDLenth = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID.GetByteCount();  // IDLenth 为 EcuID 的长度
    //        if (vctDLLprefix[j].size() < 5)
    //            continue;
    //        binTemp = String2Binary(vctDLLprefix[j][1]);   // 将 SubaDLLPrefix 的第1列转换为 CBinary
    //        if (binTemp[0] == ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID[IDLenth - 1] && vctDLLprefix[j][3] == strTemp)  
    //        {  // binTemp[0] == EcuID的最后一位  && SubaDLLPrefix 中的第3列 == SubaCANProInto 搜索的 NextID 的第3列
    //            ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SubaDLLPrefix = vctDLLprefix[j][4];
    //            ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SubaDLLPrefix_GoFile = vct_Resault[i][1];
    //            rect = m_pDiagnosisAppLayer->Entrance(ALM_DIAGNOSE);
    //            if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag && CErrorCode::EC_SUCCESS == rect)
    //                return CErrorCode::EC_SUCCESS;
    //            else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag)
    //                return  rect;
    //        }
    //    }
    //}

    return rect;
}



W_ErrorCode CSubaru::SubaCANProInto(CBinary binSearchID)   // 自动扫描
{
    W_ErrorCode rect = CErrorCode::EC_DATA;

    string strTemp;
    CBinary binTemp;
    CSearchString Search_NextID;
    CSearchString Search_SubaDLLPrefix;
    vector<string> vctTemp;
    vector<vector<string>> vctDLLprefix;
    vector<vector<string>> vct_Resault;

    if (1 == VechileStatus)  // 搜索传进的binSearchID      0x53,0x4D,0x00,0x00,0x00,0x??
        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
    else if (2 == VechileStatus)
        strTemp = GetStringValue(Subaru_Protocol_Ini_Brz_File, binSearchID);
    if (strTemp.length() <= 0)
        return CErrorCode::EC_DATA;

    CFileManager profile;
    profile.InitManager(strTemp);  // 截取字符
    vector<string> vctStr;
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = profile.GetInt(Subaru_INI_Netlayer_TTL, Subaru_INI_BaudRate_TXT); // 取Netlayer和BaudRate的值  网络层节点和波特率
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ComPin_TXT);  // 取Netlayer和ComPin的值  网络层节点和通讯引脚
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_NextID_TXT);  //  取Netlayer和NextID的值
    strTemp = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_CommTime_TXT);  // 取Netlayer和CommTime的值  网络层节点和时间参数
    SeparatorString(strTemp, ",", vctStr);
    for (W_I16 i = 0; i < vctStr.size(); i++)
        ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));

    if (1 != VechileStatus && 2 != VechileStatus)
        return CErrorCode::EC_DATA;
    if (FALSE == Search_NextID.OpenTabFile(Subaru_CANProInto_CBF))  //NEXTID查询文件->设备ID,ECU ID,进入命令
        return CErrorCode::EC_DATA;
    if (FALSE == Search_SubaDLLPrefix.OpenTabFile(Subaru_DLLPrefix_CBF))  //系统编号查询
        return CErrorCode::EC_DATA;

    if (1 == VechileStatus)  // 其他车型1,BRZ车型2
        strTemp = "00000001";
    else
        strTemp = "00000002";
    vctTemp.push_back(strTemp);
    Search_SubaDLLPrefix.SearchString(vctDLLprefix, FALSE, 0, 0, vctTemp);  // 搜索系统编号
    if (0 >= vctDLLprefix.size())
        return CErrorCode::EC_DATA;

    vctTemp.clear();
    vctTemp.push_back(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID);  // NextID
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->VehicleStatus = VechileStatus;  // 车型
    Search_NextID.SearchString(vct_Resault, FALSE, 0, 0, vctTemp);   // SubaCANProInto 中搜索NextID插入 vct_Resault中
    if (0 >= vct_Resault.size())
        return CErrorCode::EC_DATA;
    for (W_I16 i = 0; i < vct_Resault.size(); i++)  // 遍历
    {
        vctTemp = vct_Resault[i];
        if (vctTemp.size() != 7)
            continue;

        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID = String2Binary(vctTemp[5]);  // EcuID 为在 SubaCANProInto 搜索的 NextID 的第5列
        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ToolID = String2Binary(vctTemp[4]); // ToolID 为在 SubaCANProInto搜索的 NextID 的第4列
        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EnterCMD = String2Binary(vctTemp[6]);// EnterCMD 为在 SubaCANProInto 搜索的 NextID 的第6列
        strTemp = vctTemp[3];  // strTemp 为在 SubaCANProInto 搜索的 NextID 的第3列
        for (W_I16 j = 0; j < vctDLLprefix.size(); j++)
        {
            W_I16 IDLenth = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID.GetByteCount();  // IDLenth 为 EcuID 的长度
            if (vctDLLprefix[j].size() < 5)
                continue;
            binTemp = String2Binary(vctDLLprefix[j][1]);   // 将 SubaDLLPrefix 的第1列转换为 CBinary
            if (binTemp[0] == ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID[IDLenth - 1] && vctDLLprefix[j][3] == strTemp)
            {  // binTemp[0] == EcuID的最后一位  && SubaDLLPrefix 中的第3列 == SubaCANProInto 搜索的 NextID 的第3列
                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SubaDLLPrefix = vctDLLprefix[j][4];
                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SubaDLLPrefix_GoFile = vct_Resault[i][1];
                rect = m_pDiagnosisAppLayer->Entrance(ALM_DIAGNOSE);
                if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag && CErrorCode::EC_SUCCESS == rect)
                    return CErrorCode::EC_SUCCESS;
                else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag)
                    return  rect;
            }
        }
    }

    return rect;
}

/* BRZ车型的检查 菜单 */
W_ErrorCode CSubaru::DiagSoftware_BRZ()
{
    W_ErrorCode    rect = CErrorCode::EC_SUCCESS;

    string strTemp;
    CBinary binTemp, Menu1, Menu2;
    CBinary binIDBase("\x53\x00\x00\x00\x00\x00", 6);
    CTextLibrary file_BRZ_Ini;
    CFileManager profile;
    vector<string> vctStr;

    if (true != file_BRZ_Ini.OpenFile(Subaru_Protocol_Ini_Brz_File))
        return CErrorCode::EC_DATA;

    strTemp = file_BRZ_Ini.GetStringByBinary(binIDBase);
    if (strTemp.length() <= 0)
        return CErrorCode::EC_DATA;
    profile.InitManager(strTemp);
    binTemp = profile.GetBinary(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
    if (binTemp.GetByteCount() <= 0)
        return CErrorCode::EC_DATA;
    Menu1 = binTemp;
    CMenuCtrl uiMenu;
    CMenuCtrl uiMenu1;
    while (1)
    {

        W_I16 iSelect = 1;
#if defined(__NT800__) || defined(__NT5600ALLSYS__)
        iSelect = FxShowMenu(binIDBase, Menu1, uiMenu);
#endif
        if (-1 == iSelect)
            break;
        if (0 == iSelect)    // 自动扫描
        {
            if (CErrorCode::EC_SUCCESS != QuickTestProcess())
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
            }
            continue;
        }

        binTemp = binIDBase;
        binTemp.SetByteAt(4, Menu1.GetByteAt(iSelect));
        strTemp = file_BRZ_Ini.GetStringByBinary(binTemp);
        if (strTemp.length() <= 0)
            return CErrorCode::EC_DATA;
        profile.InitManager(strTemp);
        strTemp = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
        if (strTemp.length() <= 0)
            return CErrorCode::EC_DATA;
        Menu2 = String2Binary(strTemp);
        if (1 == iSelect)
        {
            Menu2 += 0xff;
        }
#ifdef    __NT5600DEF__    
        {
            CBinary MenuTemp;
            for (W_I16 i = 0; i < Menu2.GetByteCount(); i++)
            {
#ifdef    __NT5600FOUR__
                if (0x22 == Menu2[i] || 0x23 == Menu2[i] || 0x24 == Menu2[i] || 0x28 == Menu2[i])
#endif
#ifdef    __NT5600SAS__
                    if (0x2A == Menu2[i])
#endif
#ifdef    __NT5600AA__
                        if (0x24 == Menu2[i] || 0x28 == Menu2[i])
#endif
                            MenuTemp += Menu2[i];
            }
            Menu2.Clear();
            Menu2 = MenuTemp;
            MenuTemp.Clear();
        }
#endif

        while (1 == iSelect)
        {
            W_I16 iSelect1 = 0;
            if (1 < Menu2.GetByteCount())
                iSelect1 = FxShowMenu(Subaru_TTL_MNU_SUBMAIN, Menu2, uiMenu1);
            if (-1 == iSelect1)
                break;

            if (0xff == Menu2[iSelect1])
            {
                W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
                CEobdAllSystem    EobdAllSystem;

                //OBD系统只测CAN协议
                for (W_I16 i = CEobdAllSystem::PROTOCOL_BEGIN + 1; i < CEobdAllSystem::PROTOCOL_KWP; i++)
                {
                    CBinary        binCan("\x01\x00\x00\x00\x00\x00", 6);
                    binCan.SetByteAt(5, (2 + i));
                    string   strTtl = FxGetStdString(CBinary(Subaru_ID"\x00\x00\x00\x01\xFF", 6));
                    string     strCan = FxGetStdString(binCan);

                    FxShowMessageBox(strTtl, strCan, DF_MB_NOBUTTON, DT_CENTER);

                    int itemp = EobdAllSystem.SetMode(i);
                    CErrorCode::Clear();
                    if (CErrorCode::EC_SUCCESS == (ecRet = EobdAllSystem.Entrance(ALM_DIAGNOSE)))
                        break;
                }
                continue;
            }

            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
            binTemp = Subaru_CANID_BASE;
            binTemp.SetByteAt(5, Menu2.GetByteAt(iSelect1));

            if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
            {
                rect = SubaCANProInto(binTemp);
                if (CErrorCode::EC_SUCCESS != rect)
                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_COMMFAIL, DF_MB_OK, DT_LEFT);
                else if (NULL != m_pDiagnosisAppLayer)
                {
                    delete m_pDiagnosisAppLayer;
                    m_pDiagnosisAppLayer = NULL;
                }
            }
            if (1 == Menu2.GetByteCount())
                break;
        }
#ifndef    __NT800__
#ifndef __NT5600ALLSYS__
        break;
#endif
#endif
    }

    return rect;
}

/*-----------------------------------------------------------------------------
      功    能：维护功能
      参    数：iSequence ---- 系统地址
      返 回 值：错误代码
      说    明：调用特殊功能函数
-----------------------------------------------------------------------------*/

W_ErrorCode CSubaru::Service()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    W_I16            i, j, k, istrnum, iKey, iSelMenuItem;
    W_I8            iSysNum = 0;
    CMenuCtrl       munMenu1, munMenu2, munMenu3, munMenu4, munMenu5, munMenu6;
    CBinary            binTempID(Subaru_ID"\x00\x01\x00\x00\x00", 6);
    CBinary            binFirLevID(Subaru_ID"\x00\x00\x00\x37\x00", 6);
    CBinary            binSecLevID(Subaru_ID"\x00\x00\x00\x38\x00", 6);
    CBinary            bin, binPart, binrfRet;
    CBinaryGroup    bgSysId, bgTemp;
    CTextLibrary    datasheet;
    string            strdata, strPart, strItem, strMenuCond, strTemp;
    CFileManager    profile;
    vector<string>    vcttemp;
    CBinary            m_binFirstMenu;
    CBinary            m_binSecondMenu;
    CBinary            m_binThirdMenu;
    CBinaryGroup    m_bgItem;

    bool bflag = datasheet.OpenFile(Subaru_SpecFunc_Ini_File);
    VechileStatus = 1;
    bgSysId.Clear();
    bgSysId.Append(CBinary("\xC8\x00\x00\x00\x38", 5));
    //    bgSysId.Append(CBinary("\x41\x07\xde\x07\x64", 5));
    bgSysId.Append(CBinary("\x81\x00\x00\x00\x10", 5));
    bgSysId.Append(CBinary("\xC2\x00\x00\x00\x40", 5));

    if (m_iService == 0)
    {
        m_iService = 1;
        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON);

        for (j = 0; j < bgSysId.GetByteCount(); j++)
        {
            bin.Clear();
            m_binSysID.Clear();
            m_binFirstMenu = CBinary(Subaru_ID"\x00\x00\x00\x37\x00", 6);
            m_binSecondMenu = CBinary(Subaru_ID"\x00\x00\x00\x38\x00", 6);
            m_binThirdMenu.Clear();
            m_bgItem.Clear();

            for (k = 0; k < 5; k++)
                bin += bgSysId[j][k];
            m_binSysID = bin;
            binTempID.SetByteAt(1, m_binSysID[0]);
            if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
            {
                if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                    continue;

                W_I16 iMode = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->m_iMode;
                binTempID.SetByteAt(2, iMode);

                if (bflag)
                {
                    strdata = datasheet.GetStringByBinary(binTempID);
                    if (strdata.size())            //模式可能对不上
                        profile.InitManager(strdata);
                    else
                        continue;
                }
                if (2 == j)
                {
                    ProgrammingMenuSet.Append(m_binSysID);
                    break;
                }

                strPart = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_Part_TXT);        //保养功能菜单
                strItem = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_Item_TXT);        //保养功能菜单
                strMenuCond = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_MenuCond_TXT);//保养功能菜单的显示条件

                istrnum = SeparatorString(strItem, ",", vcttemp);
                for (i = 0; i < istrnum; i++)
                {
                    bin = HexString2Binary(vcttemp[i].c_str());
                    if (bin.GetByteCount())
                        m_bgItem.Append(bin);
                }

                binPart = String2Binary(strPart);
                m_binFirstMenu.SetByteAt(5, binPart[0]);
                if (binPart[0] == 0x02)
                    m_binSecondMenu.SetByteAt(5, 0x02);
                else if (binPart[0] == 0x03)
                    m_binSecondMenu.SetByteAt(5, 0x05);
                else if (binPart[0] == 0x01)
                    m_binSecondMenu.SetByteAt(5, 0x01);

                binrfRet = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->m_binrfRet;
                if (strMenuCond.size())
                {
                    W_I16            ix, jx;
                    W_I16            iNum, iSub, iMenu1, iMenu2, count;
                    vector<string>    vcttemp, vctSub;
                    CBinary            binMenu;

                    iNum = SeparatorString(strMenuCond, ",", vcttemp);
                    count = 0;
                    for (ix = 0; ix < iNum; ix++)
                    {
                        iMenu1 = vcttemp[ix].find("||");
                        iMenu2 = vcttemp[ix].find("&&");
                        if (iMenu1 > 0)
                        {
                            iSub = SeparatorString(vcttemp[ix], "||", vctSub);
                            for (jx = 0; jx < iSub; jx++)
                            {
                                binMenu = HexString2Binary(vctSub[jx]);
                                if (binrfRet.GetByteCount() < binMenu[0])
                                    continue;
                                if (binrfRet[binMenu[0]] & binMenu[1])
                                {
                                    m_binThirdMenu += 0x04;            //柴油微粒滤清器
                                    break;
                                }
                            }
                        }
                        else if (iMenu2 > 0)
                        {
                            iSub = SeparatorString(vcttemp[ix], "&&", vctSub);
                            for (jx = 0; jx < iSub; jx++)
                            {
                                binMenu = HexString2Binary(vctSub[jx]);
                                if (binrfRet.GetByteCount() < binMenu[0])
                                    continue;
                                if (!(binrfRet[binMenu[0]] & binMenu[1]))
                                    break;
                                if (jx == iSub - 1)
                                {
                                    if (count == 0)
                                    {
                                        m_binThirdMenu += CBinary("\x03\x05", 2);            //喷油器
                                        count++;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                bgTemp.Clear();
                if (0x00 != m_binFirstMenu[5])
                {
                    bgTemp.Append(m_binFirstMenu);
                    bgTemp.Append(m_binSecondMenu);
                    bgTemp.Append(m_binThirdMenu);
                    bgTemp.Append(m_binSysID);
                    for (W_I16 iCounts = 0; iCounts < m_bgItem.GetByteCount(); iCounts++)
                        bgTemp.Append(m_bgItem[iCounts]);
                    ServiceMenuSet.push_back(bgTemp);
                }
            }
        }
    }

    datasheet.CloseFile();
    if (isProgramming)
        return ecRet;
    if (ServiceMenuSet.size() <= 0)
    {
        FxShowMessageBox(CBinary("\xff\x05\x00\x00\x00\x61", 6), CBinary("\xff\x05\x00\x00\x00\x38", 6), DF_MB_OK, DT_CENTER);
        return ecRet;
    }

    while (1)
    {
        munMenu1.InitCtrl(CBinary(Subaru_ID"\x00\x00\x00\x37\x00", 6));
        for (i = 0; i < ServiceMenuSet.size(); i++)
            munMenu1.AddOneItem(ServiceMenuSet[i][0]);
        iKey = munMenu1.ShowCtrl();
        if (iKey == -1)
            break;

        if (ServiceMenuSet[iKey].GetByteCount() > 3)
        {
            m_binFirstMenu = ServiceMenuSet[iKey][0];
            m_binSecondMenu = ServiceMenuSet[iKey][1];
            m_binThirdMenu = ServiceMenuSet[iKey][2];
            m_binSysID = ServiceMenuSet[iKey][3];
            m_bgItem.Clear();
            for (W_I16 iCounts = 4; iCounts < ServiceMenuSet[iKey].GetByteCount(); iCounts++)
                m_bgItem.Append(ServiceMenuSet[iKey][iCounts]);
        }
        while (1)
        {
            munMenu2.InitCtrl(m_binFirstMenu);
            munMenu2.AddOneItem(m_binSecondMenu);
            iKey = munMenu2.ShowCtrl();
            if (iKey == -1)
                break;

            if (m_binThirdMenu.GetByteCount())
            {
                while (1)
                {
                    CBinary        binThird(Subaru_ID"\x00\x00\x00\x39\x00", 6);
                    munMenu3.InitCtrl(m_binSecondMenu);
                    for (k = 0; k < m_binThirdMenu.GetByteCount(); k++)
                    {
                        binThird.SetByteAt(5, m_binThirdMenu[k]);
                        munMenu3.AddOneItem(binThird);
                    }
                    iKey = munMenu3.ShowCtrl();
                    if (iKey == -1)
                        break;

                    binThird.SetByteAt(5, m_binThirdMenu[iKey]);
                    if (iKey == 0)            //喷油器
                    {
                        while (1)
                        {
                            munMenu4.InitCtrl(binThird);
                            munMenu4.AddOneItem(m_bgItem[0]);
                            iSelMenuItem = munMenu4.ShowCtrl();
                            if (iSelMenuItem == -1)
                                break;

                            Service_Communicate(m_bgItem[0]);
                        }
                    }
                    else if (iKey == 1)        //自适应
                    {
                        while (1)
                        {
                            munMenu5.InitCtrl(binThird);
                            munMenu5.AddOneItem(m_bgItem[1]);
                            munMenu5.AddOneItem(m_bgItem[2]);
                            iSelMenuItem = munMenu5.ShowCtrl();
                            if (iSelMenuItem == -1)
                                break;

                            if (iSelMenuItem == 0)
                                Service_Communicate(m_bgItem[1]);
                            else if (iSelMenuItem == 1)
                                Service_Communicate(m_bgItem[2]);
                        }
                    }
                    else if (iKey == 2)        //柴油微粒滤清器
                    {
                        while (1)
                        {
                            munMenu6.InitCtrl(binThird);
                            munMenu6.AddOneItem(m_bgItem[3]);
                            munMenu6.AddOneItem(m_bgItem[4]);
                            iSelMenuItem = munMenu6.ShowCtrl();
                            if (iSelMenuItem == -1)
                                break;

                            if (iSelMenuItem == 0)
                                Service_Communicate(m_bgItem[3]);
                            else if (iSelMenuItem == 1)
                                Service_Communicate(m_bgItem[4]);
                        }
                    }
                }
            }
            else
            {
                while (1)
                {
                    munMenu3.InitCtrl(m_binSecondMenu);
                    for (k = 0; k < m_bgItem.GetByteCount(); k++)
                        munMenu3.AddOneItem(m_bgItem[k]);
                    iKey = munMenu3.ShowCtrl();
                    if (iKey == -1)
                        break;

                    Service_Communicate(m_bgItem[iKey]);
                }
            }
        }
    }

    return ecRet;
}

// 常用特殊功能
W_ErrorCode CSubaru::HotFunction()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    W_I16            iSelMenuItem, k, i, j;
    CMenuCtrl        uiMenu;
    CBinary            binSearchID(Subaru_ID"\x00\x00\x00\x00\x00", 6);
    CBinary            binSubSearchId(Subaru_ID"\x00\x00\x00\x00\x00", 6);//Kas
    CBinary            binTemp, binMainMenu;
    CFileManager    profile;
    string            strMenu, strData, strTemp, strTitleID;
    CSearchString    SubaHotFunction;
    vector<string>    vctStr, vctStrMenu, vctStrFuncMenu, vctStrPro, vctSelMenu, vctSpecFuncID, vctSysProInfo, vctSysSelMenuInfo;
    vector<vector<string>>    SearchResult, SearchStrMenu;

    VechileStatus = 1;
    if (FALSE == SubaHotFunction.OpenTabFile(Subaru_SubaHotFunction_CBF))
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }
    vctStrMenu.clear();
    vctStr.clear();
    vctStr.push_back("53");
    SubaHotFunction.SearchString(SearchResult, FALSE, 0, 0, vctStr);
    for (i = 0; i < SearchResult.size(); i++)
    {
        if (SearchResult[i][7] == "1" || SearchResult[i][7] == "0")  // SearchResult[i][7] == "0"
        {
            SearchStrMenu.push_back(SearchResult[i]);
            for (k = 0; k < vctStrMenu.size(); k++)    //获取常用特殊功能的菜单并去重复处理
            {
                if (SearchResult[i][1] == vctStrMenu[k])
                    break;
            }
            if (k == vctStrMenu.size())
                vctStrMenu.push_back(SearchResult[i][1]);
        }

        if (SearchResult[i][1] == "FF0700000049")  // 制动排气
        {
            vctStrMenu.pop_back();
        }
    }

    while (1)
    {
        uiMenu.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x40", 6));
        for (i = 0; i < vctStrMenu.size(); i++)
        {
            binTemp = HexString2Binary(vctStrMenu[i]);
            uiMenu.AddOneItem(binTemp);
        }
        iSelMenuItem = uiMenu.ShowCtrl();
        if (iSelMenuItem == -1)
            return ecRet;

        strTitleID = vctStrMenu[iSelMenuItem];

        vctStrPro.clear();
        //vctStrPro.push_back("04");//CAN
        if (strTitleID == "FF070000004A")//颗粒滤清器及催化剂
            vctStrPro.push_back("02");//KWP1
        if (strTitleID == "FF070000004D")//转向
            vctStrPro.push_back("03");//KWP2

        vctStrPro.push_back("04");//CAN

        vctSelMenu.clear();
        if (strTitleID == "FF070000004D" || strTitleID == "FF0700000042")//转向  电动驻车制动
            vctSelMenu.push_back("03");
        else
        {
            if (strTitleID == "FF0700000048")  // 电池
            {
                vctSelMenu.push_back("17");
                //vctSelMenu.push_back("18");
            }

            if (strTitleID == "FF0700000055")  // 大灯
            {
                vctSelMenu.push_back("1A");
                //vctSelMenu.push_back("12");
            }

            if (strTitleID == "FF0700000052")  // 座椅
            {
                vctSelMenu.push_back("1F");
            }

            if (strTitleID == "FF0700000058")  // 胎压检测系统   协议
            {
                vctSelMenu.push_back("0C");
            }

            if (strTitleID == "FF0700000051")  // 车门，车窗
            {
                vctSelMenu.push_back("2E");
                vctSelMenu.push_back("19");
            }

            vctSelMenu.push_back("01");
            if (strTitleID == "FF070000004B")//防盗系统及钥匙
                vctSelMenu.push_back("06");
        }

        vctStrFuncMenu.clear();
        vctSpecFuncID.clear();
        vctSysProInfo.clear();
        vctSysSelMenuInfo.clear();
        for (i = 0; i < vctStrPro.size(); i++)//CAN在前 其他协议在后
        {
            for (k = 0; k < vctSelMenu.size(); k++)
            {
                m_ucSecSelItem = atoi(vctSelMenu[k].c_str());
                if (vctStrPro[i] == "04")
                {
                    m_specFuncID = 0;
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                    {
                        CBinary PtlCan;
                        binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);
                        binTemp.SetByteAt(5, m_ucSecSelItem);
                        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                        if (strTemp.length() > 0)
                        {
                            profile.InitManager(strTemp);
                            PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);  //
                        }
                        if (PtlCan.GetByteCount() > 0)
                        {
                            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                            binTemp = Subaru_CANID_BASE;
                            binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);

                            ecRet = SubaCANProInto(binTemp);
                        }
                        else
                        {
                            ecRet = CErrorCode::EC_COMMUNICATION;
                        }
                        if (NULL != m_pDiagnosisAppLayer)
                        {
                            delete m_pDiagnosisAppLayer;
                            m_pDiagnosisAppLayer = NULL;
                        }
                        if (CErrorCode::EC_SUCCESS == ecRet)
                        {
                            for (j = 0; j < SearchStrMenu.size(); j++)
                            {
                                if (strTitleID == SearchStrMenu[j][1] && vctStrPro[i] == SearchStrMenu[j][5] && vctSelMenu[k] == SearchStrMenu[j][4])
                                {
                                    vctStrFuncMenu.push_back(SearchStrMenu[j][2]);
                                    vctSpecFuncID.push_back(SearchStrMenu[j][3]);
                                    vctSysProInfo.push_back(vctStrPro[i]);
                                    vctSysSelMenuInfo.push_back(vctSelMenu[k]);
                                }
                            }
                        }
                    }
                }
                else
                {
                    CBinary PtlKwp;
                    binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);
                    binTemp.SetByteAt(5, m_ucSecSelItem);
                    strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                    if (strTemp.length() > 0)
                    {
                        profile.InitManager(strTemp);
                        if (vctStrPro[i] == "02")
                            PtlKwp = CBinary("\x81\x00\x00\x00\x10", 5);//profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp1_TXT);
                        else if (vctStrPro[i] == "03")
                            PtlKwp = CBinary("\xC1\x00\x00\x00\x28", 5);//profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp2_TXT);
                    }
                    if (PtlKwp.GetByteCount() > 0)
                    {
                        m_binSysID = PtlKwp;
                        if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                        {
                            if (CErrorCode::EC_SUCCESS == m_pDiagnosisAppLayer->Entrance())
                            {
                                for (j = 0; j < SearchStrMenu.size(); j++)
                                {
                                    if (strTitleID == SearchStrMenu[j][1] && vctStrPro[i] == SearchStrMenu[j][5] && vctSelMenu[k] == SearchStrMenu[j][4])
                                    {
                                        vctStrFuncMenu.push_back(SearchStrMenu[j][2]);
                                        vctSpecFuncID.push_back(SearchStrMenu[j][3]);
                                        vctSysProInfo.push_back(vctStrPro[i]);
                                        vctSysSelMenuInfo.push_back(vctSelMenu[k]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        CMenuCtrl    uiSubMenu;
        CBinary        binTitle = HexString2Binary(strTitleID);
        while (1)
        {
            uiSubMenu.InitCtrl(binTitle);
            for (i = 0; i < vctStrFuncMenu.size(); i++)
            {
                binTemp = HexString2Binary(vctStrFuncMenu[i]);
                uiSubMenu.AddOneItem(binTemp);
            }
            if (strTitleID == "FF0700000041")
                uiSubMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));

            W_I16    iCount = uiSubMenu.m_vctString.size();
            if (iCount == 0)
            {
                FxShowMessageBox(binTitle, CBinary("\x53\x20\x00\x00\x00\x80", 6));
                break;
            }

            iSelMenuItem = uiSubMenu.ShowCtrl();
            if (iSelMenuItem == -1)
                break;

            if (strTitleID == "FF0700000041" && (iSelMenuItem == iCount - 1))
            {
                Resetwithouttool();  //手动归零
                continue;
            }

            m_specFuncID = StringToHex(vctSpecFuncID[iSelMenuItem]);
            m_ucSecSelItem = atoi(vctSysSelMenuInfo[iSelMenuItem].c_str());
            if (vctSysProInfo[iSelMenuItem] == "04")
            {
                if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                {
                    CBinary PtlCan;
                    binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);
                    binTemp.SetByteAt(5, m_ucSecSelItem);
                    strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                    if (strTemp.length() > 0)
                    {
                        profile.InitManager(strTemp);
                        PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);
                    }
                    if (PtlCan.GetByteCount() > 0)
                    {
                        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                        binTemp = Subaru_CANID_BASE;
                        binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);

                        SubaCANProInto(binTemp);
                    }
                    if (NULL != m_pDiagnosisAppLayer)
                    {
                        delete m_pDiagnosisAppLayer;
                        m_pDiagnosisAppLayer = NULL;
                    }
                }
            }
            else
            {
                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_INFO_ESTAB_VEH_COMMUNICATION, DF_MB_NOBUTTON);
                CBinary PtlKwp;
                binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);
                binTemp.SetByteAt(5, m_ucSecSelItem);
                strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                if (strTemp.length() > 0)
                {
                    profile.InitManager(strTemp);
                    if (vctSysProInfo[iSelMenuItem] == "02")
                        PtlKwp = CBinary("\x81\x00\x00\x00\x10", 5);//profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp1_TXT);
                    else if (vctSysProInfo[iSelMenuItem] == "03")
                        PtlKwp = CBinary("\xC1\x00\x00\x00\x28", 5);//profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp2_TXT);
                }
                if (PtlKwp.GetByteCount() > 0)
                {
                    m_binSysID = PtlKwp;
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                        m_pDiagnosisAppLayer->Entrance();
                }
            }
        }
    }

    return ecRet;
}

//手动归零
W_ErrorCode CSubaru::Resetwithouttool()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    W_I16            iSelMenuItem, i, k;
    CMenuCtrl        uiMenu;
    CBinary            binTemp;
    string            strShow;
    CSearchString    SubaOilReserMT;
    vector<string>    vctStr, vctStrYearMenu, vctStrVehicleMenu;
    vector<vector<string>>    SearchResult, SearchStrSubMenu;

    if (FALSE == SubaOilReserMT.OpenTabFile(Subaru_SubaOilReserMT_CBF))
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return CErrorCode::EC_DATA;
    }
    vctStrVehicleMenu.clear();    //车型
    vctStr.clear();
    vctStr.push_back("532100000101");
    SubaOilReserMT.SearchString(SearchResult, FALSE, 0, 0, vctStr);
    for (i = 0; i < SearchResult.size(); i++)
    {
        for (k = 0; k < vctStrVehicleMenu.size(); k++)    //获取常用特殊功能的菜单并去重复处理
        {
            if (SearchResult[i][1] == vctStrVehicleMenu[k])
                break;
        }
        if (k == vctStrVehicleMenu.size())
            vctStrVehicleMenu.push_back(SearchResult[i][1]);
    }

    while (1)
    {
        uiMenu.InitCtrl(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));
        for (i = 0; i < vctStrVehicleMenu.size(); i++)
        {
            binTemp = HexString2Binary(vctStrVehicleMenu[i]);
            uiMenu.AddOneItem(binTemp);
        }
        iSelMenuItem = uiMenu.ShowCtrl();
        if (iSelMenuItem == -1)
            break;

        vctStrYearMenu.clear();   // 年份
        SearchStrSubMenu.clear();
        for (i = 0; i < SearchResult.size(); i++)
        {
            if (SearchResult[i][1] == vctStrVehicleMenu[iSelMenuItem])
            {
                SearchStrSubMenu.push_back(SearchResult[i]);
                for (k = 0; k < vctStrYearMenu.size(); k++)    //获取常用特殊功能的菜单并去重复处理
                {
                    if (SearchResult[i][2] == vctStrYearMenu[k])
                        break;
                }
                if (k == vctStrYearMenu.size())
                    vctStrYearMenu.push_back(SearchResult[i][2]);
            }
        }

        CMenuCtrl    uiYearMenu;
        while (1)
        {
            uiYearMenu.InitCtrl(CBinary("\xFF\x05\x00\x00\x00\x6C", 6));
            for (i = 0; i < vctStrYearMenu.size(); i++)
            {
                binTemp = HexString2Binary(vctStrYearMenu[i]);
                uiYearMenu.AddOneItem(binTemp);
            }
            iSelMenuItem = uiYearMenu.ShowCtrl();
            if (iSelMenuItem == -1)
                break;

            for (i = 0; i < SearchStrSubMenu.size(); i++)
            {
                if (SearchStrSubMenu[i][2] == vctStrYearMenu[iSelMenuItem])
                {
                    strShow = SearchStrSubMenu[i][7];
                    break;
                }
            }

            binTemp = HexString2Binary(strShow);
            FxShowMessageBox(CBinary("\xFF\x05\x00\x00\x00\x6C", 6), binTemp, DF_MB_OK, DT_LEFT);
        }
    }

    return ecRet;
}





// 编程
W_ErrorCode CSubaru::Programming()
{
    W_ErrorCode rect = CErrorCode::EC_SUCCESS;

    if (m_iService == 0)
        Service();
    if (ProgrammingMenuSet.GetByteCount() <= 0)
    {
        FxShowMessageBox(CBinary("\xff\x05\x00\x00\x00\x61", 6), CBinary("\xff\x05\x00\x00\x00\x38", 6), DF_MB_OK, DT_CENTER);
        return rect;
    }

    vector<CBinaryGroup> Menu1ItemSet;
    CBinaryGroup bgTemp;
    for (W_I16 i = 0; i < ProgrammingMenuSet.GetByteCount(); i++)    // 做一级菜单
    {
        bgTemp.Clear();
        if (CBinary("\xC2\x00\x00\x00\x40", 5) == ProgrammingMenuSet[i])
        {
            bgTemp.Append(STD_TTL_MNU_BODY);
            bgTemp.Append(ProgrammingMenuSet[i]);
        }
        else if (1 == 1)    // 有增加编程功能菜单时添加这里
        {

        }
        for (W_I16 i = 0; i < Menu1ItemSet.size() || 0 == Menu1ItemSet.size(); i++)
        {
            if (0 == Menu1ItemSet.size() && bgTemp.GetByteCount() > 0)
                Menu1ItemSet.push_back(bgTemp);
            else if (Menu1ItemSet[i][0] == bgTemp[0] && bgTemp.GetByteCount() > 1)
                Menu1ItemSet[i].Append(bgTemp[1]);
            else if (Menu1ItemSet.size() > 0 && i == Menu1ItemSet.size() - 1 && bgTemp.GetByteCount() > 1)
                Menu1ItemSet.push_back(bgTemp);
        }
    }
    if (Menu1ItemSet.size() <= 0)
    {
        FxShowMessageBox(CBinary("\xff\x05\x00\x00\x00\x61", 6), CBinary("\xff\x05\x00\x00\x00\x38", 6), DF_MB_OK, DT_CENTER);
        return rect;
    }

    CMenuCtrl Menu1;

    vector<CBinaryGroup> Menu2IdSet;
    CBinaryGroup bgTemp2;
    while (1)
    {
        Menu1.InitCtrl(STD_TTL_MNU_PROGRAMMING_CUSTOMIZATION);
        for (W_I16 i = 0; i < Menu1ItemSet.size(); i++)
            Menu1.AddOneItem(Menu1ItemSet[i][0]);
        W_I16 iSelect = Menu1.ShowCtrl();
        if (-1 == iSelect)
            break;

        bgTemp.Clear();
        bgTemp = Menu1ItemSet[iSelect];

        CMenuCtrl Menu2;
        while (1)
        {
            Menu2IdSet.clear();
            Menu2.InitCtrl(bgTemp[0]);
            for (W_I16 i = 1; i < bgTemp.GetByteCount(); i++)            // 做二级菜单
            {
                bgTemp2.Clear();
                if (bgTemp[i] == CBinary("\xC2\x00\x00\x00\x40", 5))    // 集成单元->集成单元定制
                {
                    bgTemp2.Append(bgTemp[i]);                            // 系统ID
                    bgTemp2.Append(CBinary("\x53\x00\x00\x00\x02\x0f", 6));
                    Menu2.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x0f", 6));    // 直接手动在文档SUBASPECFUNCINI.txt中查询添加
                    Menu2IdSet.push_back(bgTemp2);
                }
                else if (1 == 1)    // 有增加编程功能菜单时在这里添加    进入MenuAndProcess后的Switch条件也相应需要增加
                {

                }
            }
            iSelect = Menu2.ShowCtrl();
            if (-1 == iSelect)
                break;
            m_binSysID = Menu2IdSet[iSelect][0];
            Service_Communicate(Menu2IdSet[iSelect][1]);
        }
    }

    return rect;
}


W_ErrorCode CSubaru::Service_Communicate(CBinary binItem)
{
    m_specFuncID = binItem[4] * 0x100 + binItem[5];
    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON);

    if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
    {
        if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
        {
            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
        }
    }

    return 0;
}

/*-----------------------------------------------------------------------------
      功    能：分配新的应用层
      参    数：iSequence ---- 系统地址
      返 回 值：错误代码
      说    明：根据系统地址分配相应的应用层
-----------------------------------------------------------------------------*/

W_ErrorCode CSubaru::NewAppLayer(W_I16 iSequence)
{
    CAppLayerBase* pDiagnosisAppLayer = NULL;
    bool bExistSystem = false;

    W_ErrorCode ecRet = CErrorCode::EC_SUCCESS;

    if (iSequence == 0xff)
    {
        pDiagnosisAppLayer = new CSubaruBaseAllSystemApp;
        bExistSystem = true;
    }
    else if (iSequence == 0x00)
    {
        pDiagnosisAppLayer = new CSubaruBaseQuickTestApp;
        bExistSystem = true;
    }
    else if (iSequence < 0x40)
    {
        pDiagnosisAppLayer = new CSubaruBaseOldApp;
        bExistSystem = true;
    }
    else if (iSequence < 0x80)
    {
        pDiagnosisAppLayer = new CSubaruBaseCanApp;
        bExistSystem = true;
    }
    else if (iSequence < 0xc0)
    {
        pDiagnosisAppLayer = new CSubaruBaseKwp1App;
        bExistSystem = true;
    }
    else
    {
        pDiagnosisAppLayer = new CSubaruBaseKwp2App;
        bExistSystem = true;
    }

    if (bExistSystem)
    {
        if (NULL == pDiagnosisAppLayer)
            ecRet = CErrorCode::EC_ALLOCATE_MEMORY;
        else
        {
            if (m_pDiagnosisAppLayer)
                delete m_pDiagnosisAppLayer;
            m_pDiagnosisAppLayer = pDiagnosisAppLayer;
        }
    }
    else
        ecRet = CErrorCode::EC_NONE_SYSTEM;

    if (CErrorCode::EC_SUCCESS != ecRet)
        CErrorCode::SetLastError(ecRet);

    return ecRet;
}

/*-----------------------------------------------------------------------------
      功    能：取得系统类别
      参    数：binaddr ---- 系统地址
      返 回 值：返回系统类别
      说    明：无
-----------------------------------------------------------------------------*/

CBinary CSubaru::GetMenuItem(CBinary binaddr)
{
    CBinary binMenuItem;
    CBinary binId;
    CFileManager    profile;
    string        strdata;
    int i;
    if (binaddr.GetByteCount() < 5)
        return binMenuItem;
    binMenuItem.Clear();
    binId += SubaruVALUE;
    for (i = 0; i < 5; i++)
        binId += binaddr[i];
    if (1 == VechileStatus)
        strdata = GetStringValue(Subaru_Protocol_Ini_File, binId);
    else if (2 == VechileStatus)
    {
        binMenuItem += binaddr[4];
        binMenuItem += 0x00;
        return binMenuItem;
    }
    if (!strdata.size())
        return binMenuItem;
    profile.InitManager(strdata);
    strdata = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ControModel_TXT);
    binMenuItem += String2Binary(strdata);
    binMenuItem += binaddr[1];

    return binMenuItem;
}

/*-----------------------------------------------------------------------------
      功    能：自动扫描
      参    数：无
      返 回 值：错误代码
      说    明：实现自动扫描等功能
-----------------------------------------------------------------------------*/
W_ErrorCode CSubaru::QuickTestProcess()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;

    CTextLibrary    datasheet;
    CFileManager    profile;
    CBinary            binTempId("\x53\x00\x00\x00\x01\x00", 6);
    CBinary            binAllSysId;
    CBinary            binTxtId;
    CBinaryGroup    bgAllSysId;
    CBinary            binPrtl;
    CDtcScanCtrl    scanselect;
    bool            bIsContScan = true;
    bool            bIsKas = false;//kas系统处理
    bool            bKasErase = true;//Kas清码成功标志
    CBinaryGroup    EnterSuccessID;

    string            strtemp;
    string            strdata;

    W_I16            iSysNum = 0;
    W_I16            iCount;
    W_I16            iSysCount;
    W_I16            i;
    W_I16            iRet;
    W_I16            iCurNo;

    if (1 == VechileStatus)
        strdata = GetStringValue(Subaru_Protocol_Ini_File, binTempId);
    else
        strdata = GetStringValue(Subaru_Protocol_Ini_Brz_File, binTempId);
    if (!strdata.size())
    {
        ecRet = CErrorCode::EC_DATA;
        CErrorCode::SetLastError(ecRet);
        return ecRet;
    }
    profile.InitManager(strdata);
    strtemp = profile.GetItemValue(Subaru_INI_Protocol_TTL, Subaru_INI_PrtlItem_TXT);
    binPrtl = String2Binary(strtemp);
    for (i = 0; i < binPrtl.GetByteCount(); i++)
    {
        CBinary    binTemp;
        if (binPrtl[i] == Suba_Old)
        {
            strtemp = profile.GetItemValue(Subaru_INI_Protocol_TTL, Subaru_INI_ItemOld_TXT);
            binTemp += String2Binary(strtemp);
        }
        if (binPrtl[i] == Suba_Kwp1)
        {
            strtemp = profile.GetItemValue(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp1_TXT);
            binTemp += String2Binary(strtemp);
        }
        if (binPrtl[i] == Suba_Kwp2)
        {
            strtemp = profile.GetItemValue(Subaru_INI_Protocol_TTL, Subaru_INI_ItemKwp2_TXT);
            binTemp += String2Binary(strtemp);
        }
        if (binPrtl[i] == Suba_Can)
        {
            strtemp = profile.GetItemValue(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);
            binTemp += String2Binary(strtemp);
        }
        if (binTemp.GetByteCount())
        {
#ifdef __NT5600FOUR__
            CBinary binNT5600DiagElite;
            for (W_I16 k = 0; k < binTemp.GetByteCount() / 5; k++)
            {
                CBinary binAsystem;
                for (W_I16 j = 0; j < 5; j++)
                    binAsystem += binTemp.GetByteAt(k * 5 + j);

                BYTE iSystemID = binAsystem[0];

                if ((Old_ENGINE != iSystemID) && (Old_AT != iSystemID) && (Old_ABS != iSystemID)
                    && (Kwp1_ENGINE1 != iSystemID) && (Kwp1_ENGINE2 != iSystemID) && (Kwp1_ENGINE3 != iSystemID)
                    && (Kwp1_AT1 != iSystemID) && (Kwp1_AT2 != iSystemID) && (Kwp2_ABS != iSystemID)
                    && (Kwp2_SRS != iSystemID) && (Kwp2_EPS != iSystemID))
                    continue;
                binNT5600DiagElite += binAsystem;
            }
            binTemp = binNT5600DiagElite;
#endif
#ifdef __NT5600AA__
            CBinary binNT5600DiagElite;
            for (W_I16 k = 0; k < binTemp.GetByteCount() / 5; k++)
            {
                CBinary binAsystem;
                for (W_I16 j = 0; j < 5; j++)
                    binAsystem += binTemp.GetByteAt(k * 5 + j);
                BYTE iSystemID = binAsystem[0];
                if ((Kwp2_ABS != iSystemID)
                    && (Kwp2_SRS != iSystemID) && (Kwp2_EPS != iSystemID))
                    continue;
                binNT5600DiagElite += binAsystem;
            }
            binTemp = binNT5600DiagElite;
#endif
#ifdef __NT5600SAS__
            CBinary binNT5600DiagElite;
            for (W_I16 k = 0; k < binTemp.GetByteCount() / 5; k++)
            {
                CBinary binAsystem;
                for (W_I16 j = 0; j < 5; j++)
                    binAsystem += binTemp.GetByteAt(k * 5 + j);
                BYTE iSystemID = binAsystem[0];
                if ((Kwp2_EPS != iSystemID))
                    continue;
                binNT5600DiagElite += binAsystem;
            }
            binTemp = binNT5600DiagElite;
#endif
            bgAllSysId.Append(binTemp);
        }
    }

    for (i = 0; i < bgAllSysId.GetByteCount(); i++)
    {
        iSysNum += (bgAllSysId[i].GetByteCount() / 5);
        binAllSysId += bgAllSysId[i];
    }
    iCount = 0;
    iSysCount = 0;
    W_I16 NowSelectNum = 0;
    m_binAutoUnitId.Clear();
    scanselect.InitCtrl(Subaru_TTL_AUTOSCAN, true);
    scanselect.SetBtnCaption(CDtcScanCtrl::SCAN_BTN_DIS_DTC, FxGetStdString(STD_TXT_MSG_REPORT_RESULT));
    scanselect.SetScanCount(iSysNum);
    scanselect.SetButtonVisible(CDtcScanCtrl::SCAN_BTN_SAVE, false);

    CBinary binSysId;
    CBinary    binTemp(Subaru_ID, 1);
    CBinary binSysTTl(Subaru_ID"\x00\x00\x00\x01\x00", 6);
    while (1)
    {
        binSysId.Clear();
        binTemp = CBinary(Subaru_ID, 1);
        binSysTTl = CBinary(Subaru_ID"\x00\x00\x00\x01\x00", 6);

        if ((iCount < iSysNum))
        {
            if (bIsContScan)
            {
                for (i = 0; i < 5; i++)
                    binSysId += binAllSysId[5 * iCount + i];
                scanselect.SetCurScanIndex(iCount + 1);
                binTxtId = GetMenuItem(binSysId);

                //if(binTxtId.GetByteCount())
                binSysTTl.SetByteAt(5, binTxtId[0]);
                binTemp += binSysId;
                if (m_binAutoUnitId.GetByteCount())
                {
                    if (CompareUnit(binTemp))
                    {
                        iCount++;
                        continue;
                    }
                }
                scanselect.AddOneItem(binSysTTl);
                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST1);
            }
        }
        else
        {
            scanselect.FinishScan();
        }

        if ((iCount >= iSysNum) && (iSysCount == 0))
        {
            ecRet = CErrorCode::EC_COMMUNICATION;
            CErrorCode::SetLastError(ecRet);
            break;
        }
        iRet = scanselect.ShowCtrl();
        if (iRet == -1)
        {
            ecRet = CErrorCode::EC_SUCCESS;
            break;
        }
        if (iRet >= 0)
        {
            ShowCommWaitInfo();
            iCurNo = scanselect.GetCurSelectItem();
            if (iCurNo < iSysCount)
            {
                if (m_vcttoken.size() > iCurNo)
                {
                    m_token = m_vcttoken[iCurNo];
                    m_binSysID = m_token.binSysId;
                }
                if (EnterSuccessID.GetByteCount() > iCurNo)
                {
                    m_binSysID = EnterSuccessID[iCurNo];
                }
                if (m_binSysID.GetByteCount() <= 0)
                    continue;
                if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                {
                    if (CAN_HEAD == m_binSysID[0])
                    {
                        CBinary binSearch("\x53", 1);
                        binSearch += m_binSysID;
                        SubaCANProInto(binSearch);
                        ecRet = CErrorCode::EC_SUCCESS;
                    }
                    else if (m_token.binSysId.GetByteCount() > 0 && m_token.binSysId.GetByteAt(0) == Kwp2_KAS)
                    {
                        CBinary binKas(Subaru_ID"\x00\x00\x00\x17\x00", 6);
                        CMenuCtrl kasMmnu;
                        CBinary    binKasMenu;
                        W_I16    iKasSel;
                        for (i = 0; i < m_vctkastoken.size(); i++)
                        {
                            m_kastoken = m_vctkastoken[i];
                            //if(m_kastoken.iDtcNum)
                            binKasMenu += m_kastoken.iSysNum;
                        }
                        while (1)
                        {
                            iKasSel = FxShowMenu(binKas, binKasMenu, kasMmnu);
                            if (iKasSel == -1)
                                break;
                            m_kastoken = m_vctkastoken[iKasSel];
                            //m_iKas = m_kastoken.iSysNum;
                            m_ucKasSelItem = m_kastoken.iSysNum;
                            ShowCommWaitInfo();
                            ecRet = m_pDiagnosisAppLayer->Entrance();
                            if (ecRet != CErrorCode::EC_SUCCESS)
                                break;
                        }
                    }
                    else
                        ecRet = m_pDiagnosisAppLayer->Entrance();
                    if (ecRet != CErrorCode::EC_SUCCESS)
                        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                    /*if(CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                    {
                        FxShowMessageBox(STD_TTL_MSG_INFORMATION,STD_TXT_ENTERSYSTEMFAIL,DF_MB_OK,DT_LEFT);
                    }*/
                    else
                    {
                        /*
                        20090212 JIANRONG新增
                        目的：进入系统后，退出系统时再重新快速读取此系统，是不是还存在故障码。
                        */
                        scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST7);
                        scanselect.StartClearDtc();
                        scanselect.ShowCtrl();
                        if (CErrorCode::EC_SUCCESS == NewAppLayer(0x00))
                        {
                            //m_binSysID = binSysId;
                            W_ErrorCode    ecRet;
                            if (m_binSysID[0] == Kwp2_KAS)//KAS系统特殊处理
                            {
                                for (i = 0; i < m_vctkastoken.size(); i++)
                                {
                                    m_iKas = m_vctkastoken[i].iSysNum;//保存KAS子系统号
                                    ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_READ_DTC);
                                    if (ecRet != CErrorCode::EC_SUCCESS)
                                        break;
                                    else
                                    {
                                        m_kastoken = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->kas_token);
                                        m_vctkastoken[i] = m_kastoken;
                                    }
                                }
                            }
                            else if (CAN_HEAD == m_binSysID[0])
                            {
                                if (CErrorCode::EC_SUCCESS == NewAppLayer(CAN_HEAD))
                                {
                                    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = Auto_Scan;
                                    CBinary binSearchID("\x53", 1);
                                    binSearchID += m_binSysID;
                                    ecRet = SubaCANProInto(binSearchID);
                                    if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag)
                                        ecRet = CErrorCode::EC_SUCCESS;
                                    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = 0;
                                }
                            }
                            else
                                ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_READ_DTC);
                            if (ecRet == CErrorCode::EC_COMMUNICATION)
                            {
                                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                            }
                            else if (CAN_HEAD == m_binSysID[0] && m_binSysID.GetByteCount() > 4)
                            {
                                vector<vector<string>> CanCodeTemp;
                                if (ecRet == CErrorCode::EC_SUCCESS)
                                {
                                    CanCodeTemp = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->DtcSet;
                                    CANSysDTCSave[m_binSysID[4]] = CanCodeTemp;
                                    CanSysNum[m_binSysID[4]] = ((CSubaruBaseCanApp*)m_pDiagnosisAppLayer)->SystemNum;
                                    m_vcttoken[iCurNo].iDtcNum = CanCodeTemp.size();
                                    if (CanCodeTemp.size() > 0)
                                    {
                                        scanselect.SetHaveDtc(iRet, true);//设置有故障码
                                        string strtemp;
                                        char   chvalue[10];
                                        strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                        sprintf(chvalue, "%d", CanCodeTemp.size());
                                        strtemp += chvalue;
                                        scanselect.SetItemValue(iRet, strtemp);
                                    }
                                    else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                                    {
                                        scanselect.SetHaveDtc(iRet, false);//设置无故障码
                                        scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST2);
                                    }
                                    else
                                    {
                                        scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST4);
                                    }
                                }
                            }
                            else
                            {
                                m_token = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->token);
                                if (m_binSysID[0] == Kwp2_KAS)
                                {
                                    for (i = 0; i < m_vctkastoken.size(); i++)
                                        m_token.iDtcNum += m_vctkastoken[i].iDtcNum;
                                }
                                m_vcttoken[iRet] = m_token;
                                if (m_token.SysFlag)
                                {
                                    if (m_token.iDtcNum == 0)
                                    {
                                        scanselect.SetHaveDtc(iRet, false);//设置无故障码
                                        if ((m_token.ReadFlag) && (m_token.SysMode))//进入，无故障码
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST2);
                                        else if ((!m_token.ReadFlag) && (m_token.SysMode))//进入，不能读码
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST4);
                                        else if (!m_token.SysMode)//进入 找不到模式
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST5);
                                    }
                                    else if (m_token.iDtcNum != 0)
                                    {
                                        scanselect.SetHaveDtc(iRet, true);//设置无故障码
                                        string strtemp;
                                        char   chvalue[10];
                                        strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                        sprintf(chvalue, "%d", m_token.iDtcNum);
                                        strtemp += chvalue;
                                        scanselect.SetItemValue(iRet, strtemp);
                                    }
                                    else
                                        scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST4);
                                }
                            }
                        }
                        scanselect.FinishClearDtc();
                    }
                }
            }
        }
        if (iRet == PAUSE)
        {
            bIsContScan = false;
            scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST6);
            continue;
        }
        if (iRet == CONTINUE)
        {
            bIsContScan = true;
            //iCount --;
            if (iCount >= iSysCount)
                scanselect.DeleteOneItem(iSysCount);
            continue;
        }
        if (iRet == SAVE)
        {
            CInputCtrl        input;
            CVehicleSave    VehicleSave;
            string strvechilename;
            string strdata;

            //string strMask ="AAAAAAAAAAAAAAAAAAAAAAAAA";
            string strMask = "&&&&&&&&&&&&&&&&&&&&&&&&&";
            string strDefault = "Subaru";
            if (KEY_PRESS_F3 == input.ShowCtrl(STD_TTL_MSG_SAVEHICHILEREC, STD_TXT_MSG_SAVEHICHILEREC, strMask, strDefault))
                continue;
            else
                strvechilename = input.GetStringValue();
            if (!strvechilename.size())
                continue;
            for (i = 0; i < iSysCount; i++)
            {
                char chvalue[10];
                m_token = m_vcttoken[i];
                m_binSysID = m_token.binSysId;
                for (int j = 0; j < 5; j++)
                {
                    sprintf(chvalue, "%02X", m_binSysID[j]);
                    strdata += chvalue;
                }
            }
            if (VehicleSave.Save(strvechilename, strdata))
                FxShowMessageBox(Subaru_TTL_SAVE, Subaru_TXT_SAVESUCCESS, DF_MB_ANYKEYRETURN);

        }
        if (iRet == QUICK_ERASE)
        {
            scanselect.StartClearDtc();
            for (i = 0; i < m_vcttoken.size(); i++)
            {
                if (m_vcttoken[i].iDtcNum)
                {
                    scanselect.SetItemValue(i, Subaru_TXT_MSG_Erasing);
                    iRet = scanselect.ShowCtrl();
                    if (m_vcttoken.size() > i)
                    {
                        m_token = m_vcttoken[i];
                        m_binSysID = m_token.binSysId;
                    }
                    if (EnterSuccessID.GetByteCount() > i)
                    {
                        m_binSysID = EnterSuccessID[i];
                    }
                    if (m_binSysID.GetByteCount() <= 0)
                        continue;
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x00))
                    {
                        if (m_binSysID[0] == Kwp2_KAS)//存在故障码，则发清码命令，否则不发清码命令.只要存在一条子系统清码失败，则系统报清码失败
                        {
                            for (int j = 0; j < m_vctkastoken.size(); j++)
                            {
                                m_iKas = m_vctkastoken[j].iSysNum;//子系统号
                                if (m_vctkastoken[j].iDtcNum)
                                {
                                    ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_CLEAR_DTC);
                                    if (CErrorCode::EC_SUCCESS != ecRet)
                                        break;
                                    else
                                    {
                                        m_kastoken = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->kas_token);
                                        m_vctkastoken[j] = m_kastoken;
                                    }
                                }
                            }
                        }
                        else if (CAN_HEAD == m_binSysID[0])
                        {
                            if (CErrorCode::EC_SUCCESS == NewAppLayer(CAN_HEAD))
                            {
                                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = Auto_Scan;
                                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_quick_Erase = 0xff;
                                CBinary binSearchID("\x53", 1);
                                binSearchID += m_binSysID;
                                ecRet = SubaCANProInto(binSearchID);
                                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_quick_Erase = 0x00;
                                ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = 0;
                            }
                        }
                        else
                            ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_CLEAR_DTC);
                        if (CErrorCode::EC_SUCCESS != ecRet)
                            scanselect.SetItemValue(i, Subaru_TXT_ErasureFailed);
                        else if (CAN_HEAD == m_binSysID[0] && m_binSysID.GetByteCount() > 4)    //can
                        {
                            vector<vector<string>> CanCodeTemp;
                            CanCodeTemp = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->DtcSet;

                            CANSysDTCSave[m_binSysID[4]] = CanCodeTemp;
                            CanSysNum[m_binSysID[4]] = ((CSubaruBaseCanApp*)m_pDiagnosisAppLayer)->SystemNum;
                            m_vcttoken[i].iDtcNum = CanCodeTemp.size();
                            if (CanCodeTemp.size() > 0)
                            {
                                scanselect.SetHaveDtc(i, true);//设置有故障码
                                string strTemp;
                                char   chvalue[10];
                                strTemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                sprintf(chvalue, "%d", CanCodeTemp.size());
                                scanselect.SetItemValue(i, strTemp + chvalue);
                            }
                            else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                            {
                                scanselect.SetHaveDtc(i, false);//设置无故障码
                                scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST2);
                            }
                            else
                                scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST4);
                        }
                        else
                        {
                            m_token = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->token);
                            if (m_binSysID[0] == Kwp2_KAS)
                            {
                                for (int j = 0; j < m_vctkastoken.size(); j++)
                                    m_token.iDtcNum += m_vctkastoken[j].iDtcNum;
                            }
                            m_vcttoken[i] = m_token;
                            if (m_token.SysFlag)
                            {
                                if (m_token.iDtcNum == 0)
                                {
                                    scanselect.SetHaveDtc(i, false);//设置无故障码
                                    if ((m_token.ReadFlag) && (m_token.SysMode))//进入，无故障码
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST2);
                                    else if ((!m_token.ReadFlag) && (m_token.SysMode))//进入，不能读码
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST4);
                                    else if (!m_token.SysMode)//进入 找不到模式
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST5);
                                }
                                else if (m_token.iDtcNum != 0)
                                {
                                    scanselect.SetHaveDtc(i, true);//设置故障码
                                    string strtemp;
                                    char   chvalue[10];
                                    strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                    sprintf(chvalue, "%d", m_token.iDtcNum);
                                    strtemp += chvalue;
                                    scanselect.SetItemValue(i, strtemp);
                                }
                                else
                                    scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST4);
                            }
                        }
                    }
                }
            }
            scanselect.FinishClearDtc();
        }
        if (iRet == DISPLAY_FAULT)
        {
            DtcShow();
        }
        if ((iCount < iSysNum) && (bIsContScan))
        {
            if (CErrorCode::EC_SUCCESS == NewAppLayer(0x00))
            {
                m_binSysID = binSysId;
                W_ErrorCode    ecRet;
                if (m_binSysID[0] == Kwp2_KAS)//KAS系统特殊处理
                {
                    for (i = 1; i <= 3; i++)
                    {
                        m_iKas = i;//保存KAS子系统号
                        ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_READ_DTC);
                        if (ecRet == CErrorCode::EC_SUCCESS)
                        {
                            scanselect.SetButtonVisible(CDtcScanCtrl::SCAN_BTN_SAVE, true);
                            m_kastoken = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->kas_token);
                            m_vctkastoken.push_back(m_kastoken);
                            bIsKas = true;
                        }
                    }
                    if (bIsKas)
                        ecRet = CErrorCode::EC_SUCCESS;
                }
                else if (CAN_HEAD == m_binSysID[0])    //can
                {
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(CAN_HEAD))
                    {
                        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = Auto_Scan;
                        CBinary binSearchID("\x53", 1);
                        binSearchID += m_binSysID;
                        ecRet = SubaCANProInto(binSearchID);
                        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->m_AutoScan = 0;
                    }
                }
                else
                    ecRet = m_pDiagnosisAppLayer->Entrance(ALM_QUICK_READ_DTC);
                if (ecRet == CErrorCode::EC_COMMUNICATION)
                {
                    scanselect.DeleteOneItem(iSysCount);
                }
                else if (CAN_HEAD == m_binSysID[0] && m_binSysID.GetByteCount() > 4)    //can
                {
                    vector<vector<string>> CanCodeTemp;
                    if (ecRet != CErrorCode::EC_SUCCESS)
                        scanselect.DeleteOneItem(iSysCount);
                    else
                    {
                        strSysName.push_back(FxGetStdString(binSysTTl));
                        EnterSuccessID.Append(m_binSysID);
                        CSubaruBaseApp::TOKEN tokenTemp;
                        tokenTemp.binSysId = m_binSysID;
                        CanCodeTemp = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->DtcSet;
                        tokenTemp.iDtcNum = CanCodeTemp.size();
                        m_vcttoken.push_back(tokenTemp);
                        CANSysDTCSave[m_binSysID[4]] = CanCodeTemp;
                        CanSysNum[m_binSysID[4]] = ((CSubaruBaseCanApp*)m_pDiagnosisAppLayer)->SystemNum;
                        if (CanCodeTemp.size() > 0)
                        {
                            scanselect.SetHaveDtc(iSysCount, true);//设置有故障码
                            string strtemp;
                            char   chvalue[10];
                            strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                            sprintf(chvalue, "%d", CanCodeTemp.size());
                            strtemp += chvalue;
                            scanselect.SetItemValue(iSysCount, strtemp);
                        }
                        else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                        {
                            scanselect.SetHaveDtc(iSysCount, false);//设置无故障码
                            scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST2);
                        }
                        else
                            scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST4);
                        iSysCount++;
                    }
                }
                else
                {
                    scanselect.SetButtonVisible(CDtcScanCtrl::SCAN_BTN_SAVE, true);
                    CBinary binUnit;
                    binUnit = GetSearchedUnit(binTemp);
                    m_binAutoUnitId += binUnit;
                    m_token = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->token);
                    if (Kwp2_KAS == m_binSysID.GetByteAt(0))//KAS系统处理
                    {
                        for (i = 0; i < m_vctkastoken.size(); i++)
                        {
                            m_kastoken = m_vctkastoken[i];
                            m_token.iDtcNum += m_kastoken.iDtcNum;//得到故障码总数
                        }
                    }
                    if (bIsKas)
                        m_token.SysFlag = true;
                    m_vcttoken.push_back(m_token);
                    if (m_token.SysFlag)
                    {
                        strSysName.push_back(FxGetStdString(binSysTTl));
                        if (m_token.iDtcNum == 0)
                        {
                            scanselect.SetHaveDtc(iSysCount, false);//设置无故障码
                            if ((m_token.ReadFlag) && (m_token.SysMode))//进入，无故障码
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST2);
                            else if ((!m_token.ReadFlag) && (m_token.SysMode))//进入，不能读码
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST4);
                            else if (!m_token.SysMode)//进入 找不到模式
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST5);
                        }
                        else//进入，有故障码
                        {
                            scanselect.SetHaveDtc(iSysCount, true);//设置有故障码
                            string strtemp;
                            char   chvalue[10];
                            strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                            sprintf(chvalue, "%d", m_token.iDtcNum);
                            strtemp += chvalue;
                            scanselect.SetItemValue(iSysCount, strtemp);
                        }
                        iSysCount++;
                        EnterSuccessID.Append(m_binSysID);
                    }
                    else
                        scanselect.DeleteOneItem(iSysCount);
                }
            }
            iCount++;
        }
    }

    return ecRet;
}

/*-----------------------------------------------------------------------------
      功    能：得到字符串
      参    数：strfilename ---- 文件名
                bintempid   ---- 文本ID
      返 回 值：字符串
      说    明：根据文件中的文本ID，得到相应的字符串
-----------------------------------------------------------------------------*/
string CSubaru::GetStringValue(string strfilename, CBinary bintempid)
{
    CTextLibrary    datasheet;
    string        strdata;

    bool status = datasheet.OpenFile(strfilename);

    strdata = datasheet.GetStringByBinary(bintempid);
    datasheet.CloseFile();
    if (!strdata.size())
    {
        CErrorCode::SetLastError(CErrorCode::EC_DATA);
        return strdata;
    }

    return strdata;
}

void CSubaru::DtcShow()
{
    CBinary binTemp;
    string strTemp;
    char buf[6];
    CTroubleCodeCtrl troublecode;
    troublecode.InitCtrl(STD_TXT_MSG_REPORT_RESULT_REPORT);
    troublecode.m_bReportMode = true;
    troublecode.AddOneItem(FxGetStdString(STD_TXT_MSG_REPORT_Status), FxGetStdString(STD_TXT_MSG_REPORT_SYSNAME), FxGetStdString(STD_TXT_MSG_REPORT_DTCCOUNT));

    for (W_I16 i = 0; i < m_vcttoken.size() && i < strSysName.size(); i++)
    {
        if (m_vcttoken[i].iDtcNum > 0)
            binTemp = STD_TXT_MSG_REPORT_FAULT;
        else
            binTemp = STD_TXT_MSG_REPORT_PASS;
        strTemp = itoa(m_vcttoken[i].iDtcNum, buf, 10);
        troublecode.AddOneItem(FxGetStdString(binTemp), strSysName[i], strTemp);
    }

    for (W_I16 i = 0; i < m_vcttoken.size() && i < strSysName.size(); i++)
    {
        if (m_vcttoken[i].iDtcNum == 0)
            continue;

        binTemp = STD_TXT_MSG_REPORT_FAULT;
        strTemp = itoa(m_vcttoken[i].iDtcNum, buf, 10);
        troublecode.AddOneItem("", "", "");
        troublecode.AddOneItem(FxGetStdString(binTemp), strSysName[i], strTemp);
        troublecode.AddOneItem(FxGetStdString(STD_TXT_MSG_REPORT_DTCCODE), FxGetStdString(STD_TXT_MSG_REPORT_DTC_CONTENT), FxGetStdString(STD_TXT_MSG_REPORT_DTCSTATUS));
        m_token = m_vcttoken[i];
        if (m_token.binSysId[0] < 0x40)
            DtcOldShow(troublecode);
        else if (m_token.binSysId[0] < 0x80 && m_token.binSysId.GetByteCount() > 4)
            DtcCanShow(m_token.binSysId[4], troublecode);
        else if (m_token.binSysId[0] < 0xc0)
            DtcKwp1Show(troublecode);
        else
        {
            if (m_token.binSysId[0] == Kwp2_KAS)
                DtcKwp2KasShow(troublecode);
            else
                DtcKwp2Show(troublecode);
        }
    }
    troublecode.ShowCtrl();
}

void CSubaru::DtcKwp2KasShow(CTroubleCodeCtrl& troublecode)
{
    W_I16 i;
    for (i = 0; i < m_kastoken.bgDtcID.GetByteCount(); i++)
        troublecode.AddOneItem(m_kastoken.bgDtcID[i], m_kastoken.bgDtcStat[i]);
}

void CSubaru::DtcOldShow(CTroubleCodeCtrl& troublecode)
{
    W_I16 i;
    if (m_token.CalBit)
    {
        for (i = 0; i < m_token.bgDtcID.GetByteCount(); i++)
            troublecode.AddOneItem(m_token.bgDtcID[i], "");
    }
    else
    {
        if (m_token.iMode == 1)
            troublecode.SetDtcCallBackFunction(CallBackOneTroubleCode);
        else
            troublecode.SetDtcCallBackFunction(CallBackTroubleCode);
        if (m_token.bLatest)
        {
            if (m_token.iMode == 0x01 && m_token.bgDtcRecv[1][2] != 0x00 && m_token.bgDtcRecv[1][2] != 0x01 && m_token.bgDtcRecv[1][2] != 0x02)//abs模式1故障码状态处理
                troublecode.AddOneItem(m_token.bgDtcID[0], Subaru_MSG_Reference);
            else
                troublecode.AddOneItem(m_token.bgDtcID[0], Subaru_MSG_Latest);
        }
        if (m_token.bOld)
        {
            if (m_token.iMode == 0x01 && m_token.bgDtcRecv[1][2] != 0x00 && m_token.bgDtcRecv[1][2] != 0x01)//取故障码状态
                troublecode.AddOneItem(m_token.bgDtcID[1], Subaru_MSG_Reference);
            else
                troublecode.AddOneItem(m_token.bgDtcID[1], Subaru_MSG_Old);
        }
        if (m_token.bOlder)
        {
            if (m_token.iMode == 0x01 && m_token.bgDtcRecv[1][2] != 0x00)
                troublecode.AddOneItem(m_token.bgDtcID[2], Subaru_MSG_Reference);
            else
                troublecode.AddOneItem(m_token.bgDtcID[2], Subaru_MSG_Older);
        }
    }
}

/*-----------------------------------------------------------------------------
      功    能：显示KWP1 系统故障码
      参    数：无
      返 回 值：无
      说    明：无
-----------------------------------------------------------------------------*/
void CSubaru::DtcKwp1Show(CTroubleCodeCtrl& troublecode)
{
    W_I16 i;
    for (i = 0; i < m_token.bgDtcID.GetByteCount(); i++)
        troublecode.AddOneItem(m_token.bgDtcID[i], "");
}

/*-----------------------------------------------------------------------------
功    能：显示Can 系统故障码
参    数：无
返 回 值：无
说    明：无
-----------------------------------------------------------------------------*/
void CSubaru::DtcCanShow(W_I16 SysID, CTroubleCodeCtrl& uiTrouble)
{
    CSearchString SubaDTC;
    vector<string> vecStr;
    vector<vector<string>> SearchResault;
    CBinary binTemp;
    string strTemp;
    string SystemNumTemp;
    vector<vector<string> > DtcTemp;

    SystemNumTemp = CanSysNum[SysID];
    DtcTemp = CANSysDTCSave[SysID];
    if (FALSE == SubaDTC.OpenTabFile(Subaru_DTC_CBF))
        return;

    vecStr.push_back(SystemNumTemp);
    SubaDTC.SearchString(SearchResault, FALSE, 0, 0, vecStr);

    for (W_I16 i = 0; i < DtcTemp.size(); i++)
    {
        binTemp = CBinary(STD_STANDARD"\x13", 6);    //请参考车辆维修手册
        for (W_I16 j = 0; j < SearchResault.size(); j++)
        {
            if (CompareStringIsEquation(DtcTemp[i][0], SearchResault[j][1]))
            {
                binTemp = String2Binary(SearchResault[j][3]);
                break;
            }
        }
        strTemp = DtcTemp[i][0].at(0);
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
        for (W_I16 j = 1; j < DtcTemp[i][0].length(); j++)
            strTemp += DtcTemp[i][0].at(j);

        uiTrouble.AddOneItem(strTemp, FxGetStdString(binTemp), DtcTemp[i][1]);
    }

    return;
}
/*-----------------------------------------------------------------------------
功    能：显示KWP2 系统故障码
参    数：无
返 回 值：无
说    明：无
-----------------------------------------------------------------------------*/
void CSubaru::DtcKwp2Show(CTroubleCodeCtrl& troublecode)
{
    CBinary            binDtcID(Subaru_ID"\x00\x00\x00\x00\x00", 6);
    CBinary         binDtcStat;//故障码状态

    W_I16            i;
    W_I16           iDtcLen;
    W_I16            iLen = 3;
    W_I16           iBin;

    CBinary            binDtcBuff;
    W_I16            iDtcStat;

    if (m_token.binDtcBuff.GetByteCount())
    {
        binDtcBuff = m_token.binDtcBuff;
        iDtcStat = m_token.iDtcStat;
    }
    iDtcLen = m_token.iDtcLen;
    binDtcID.SetByteAt(1, m_token.binSysId[0]);
    binDtcID.SetByteAt(2, m_token.iMode);
    if (m_token.binSysId[0] == Kwp2_SRS)
        troublecode.SetDtcCallBackFunction(CallBackOneTroubleCode);
    else
        troublecode.SetDtcCallBackFunction(CallBackTroubleCode);
    for (i = 0; i < m_token.iDtcNum; i++)
    {
        for (int j = 0; j < iDtcLen; j++)//如果故障码由1个字节决定，则(ID)只由第4个字节来确定故障码
        {
            W_I16 iSize = 4 + j;
            if (iSize < 6)
                binDtcID.SetByteAt((4 + j), binDtcBuff[iDtcLen * i + j]);
        }
        if (iDtcStat)
        {
            iBin = binDtcBuff[iDtcLen * i + 2];//决定故障码状态值
            if (iBin == 0xfe)//需对协议
                binDtcStat = Subaru_MSG_Current;
            else
            {
                if (m_token.binSysId[0] != Kwp2_ABS)
                    binDtcStat = Subaru_MSG_Thepast;
                else
                    binDtcStat = Subaru_MSG_Old;
            }
            troublecode.AddOneItem(binDtcID, binDtcStat);
        }
        else
            troublecode.AddOneItem(binDtcID, "");
    }
}

/*-----------------------------------------------------------------------------
      功    能：比较系统类别
      参    数：binaddr ----系统地址
      返 回 值：真或假
      说    明：无
-----------------------------------------------------------------------------*/

bool CSubaru::CompareUnit(CBinary binaddr)
{
    CBinary bin;
    CFileManager profile;
    string strdata;
    if (1 == VechileStatus)
        strdata = GetStringValue(Subaru_Protocol_Ini_File, binaddr);
    else if (2 == VechileStatus)
        strdata = GetStringValue(Subaru_Protocol_Ini_Brz_File, binaddr);
    if (!strdata.size())
        return false;
    profile.InitManager(strdata);
    strdata = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ControModel_TXT);
    bin = String2Binary(strdata);
    if (bin.GetByteCount() != 1)
        return false;
    else
    {
        for (int i = 0; i < m_binAutoUnitId.GetByteCount(); i++)
        {
            if (m_binAutoUnitId[i] == bin[0])
                return true;
        }
    }
    return false;
}

void CSubaru::ShowCommWaitInfo()//显示通讯等候信息
{
    string str1, str2, str3;
    str1 = FxGetStdString(Subaru_TTL_MNU_SEARCHID);
    str2 = FxGetStdString(STD_TXT_MSG_ESTABILISHCOMM);
    //str3 = FxGetStdString(Subaru_TXT_WAITFOR);
    //str2 += str3;
    FxShowMessageBox(str1, str2, DF_MB_NOBUTTON);
}
/*-----------------------------------------------------------------------------
      功    能：得到系统的类别
      参    数：binaddr ---- 系统地址
      返 回 值：系统类别
      说    明：无
-----------------------------------------------------------------------------*/
CBinary CSubaru::GetSearchedUnit(CBinary binaddr)
{
    CBinary bin;
    CBinary binId(Subaru_ID, 1);
    CFileManager profile;
    string strdata;
    binId += binaddr;
    if (1 == VechileStatus)
        strdata = GetStringValue(Subaru_Protocol_Ini_File, binaddr);
    else
        strdata = GetStringValue(Subaru_Protocol_Ini_Brz_File, binaddr);
    if (!strdata.size())
        return bin;
    profile.InitManager(strdata);
    strdata = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ControModel_TXT);
    bin = String2Binary(strdata);
    return bin;
}

W_ErrorCode CSubaru::SubaruVehicleRecord()
{
    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
    CVehicleComm    EcuInterface;
    CVehicleSave    VehicleSave;//保存的车型
    CMenuCtrl            mnuMenu, mnuSubMenu;
    CBinary            binMnuTTL(Subaru_ID"\x00\x00\x00\x00\x03", 6);

    W_I32            iVehicleNum;
    W_I32            i, iSelectItem;
    W_FLOAT            iVoltage = 0;
    string            strMenuTxt;

    iVehicleNum = VehicleSave.GetCount();
    while (1)
    {
        CBinary    binVehicle;
        string    strVehicledata;
        mnuMenu.InitCtrl(binMnuTTL);
        for (i = 0; i < iVehicleNum; i++)
        {
            strMenuTxt = "";
            strMenuTxt = VehicleSave.GetName(i);
            mnuMenu.AddOneItem(strMenuTxt);
        }
        iSelectItem = mnuMenu.ShowCtrl();
        Sleep(100);
        if (iSelectItem == -1)
            break;
        if (iSelectItem >= 0)
        {
            CBinary binSysTTl(Subaru_ID"\x00\x00\x00\x01\x00", 6);
            CBinary    binTxtId;
            CBinary binSysId;
            W_I16   iSecSubItem;

            strVehicledata = VehicleSave.GetItemValue(iSelectItem);
            binVehicle = String2Binary(strVehicledata);
            if ((binVehicle.GetByteCount() % 5) != 0)
                return ecRet = CErrorCode::EC_DATA;
            while (1)
            {
                W_I16 iLoop = 1;
                mnuSubMenu.InitCtrl(VehicleSave.GetName(iSelectItem));
                for (i = 0; i < binVehicle.GetByteCount() / 5; i++)
                {
                    binSysId.Clear();
                    for (int j = 0; j < 5; j++)
                        binSysId += binVehicle[5 * i + j];
                    binTxtId = GetMenuItem(binSysId);
                    binSysTTl.SetByteAt(5, binTxtId[0]);
                    mnuSubMenu.AddOneItem(binSysTTl);
                }
                iSecSubItem = mnuSubMenu.ShowCtrl();
                Sleep(100);
                if (iSecSubItem == -1)
                    break;
                if (iSecSubItem >= 0)
                {
                    binSysId.Clear();
                    for (int j = 0; j < 5; j++)
                        binSysId += binVehicle[5 * iSecSubItem + j];
                    m_binSysID = binSysId;
                    if (m_binSysID[0] == Kwp2_KAS)
                    {
                        CBinary binMenu;
                        CBinary binKasMenu(Subaru_ID"\x00\x00\x00\x17\x00", 6);
                        CMenuCtrl mmMenu;
                        W_I16 iSecKas;
                        m_vctkastoken.clear();
                        ShowCommWaitInfo();
                        if (CErrorCode::EC_SUCCESS == NewAppLayer(0x00))
                        {
                            W_ErrorCode    ecRet;
                            for (i = 1; i <= 3; i++)
                            {
                                m_iKas = i;//保存KAS子系统号
                                ecRet = m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM);
                                if (ecRet == CErrorCode::EC_SUCCESS)
                                {
                                    m_kastoken = *(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->kas_token);
                                    m_vctkastoken.push_back(m_kastoken);
                                }
                            }
                            for (i = 0; i < m_vctkastoken.size(); i++)
                            {
                                m_kastoken = m_vctkastoken[i];
                                if (m_kastoken.SysFlag)
                                    binMenu += m_kastoken.iSysNum;
                            }
                            if (m_vctkastoken.size())
                            {
                                while (1)
                                {
                                    iSecKas = FxShowMenu(binKasMenu, binMenu, mmMenu);
                                    if (iSecKas == -1)
                                        break;
                                    m_ucKasSelItem = binMenu[iSecKas];
                                    ShowCommWaitInfo();
                                    if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                    {
                                        if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                        {
                                            CBinary binSearchID(Subaru_ID, 1);
                                            string  strData;
                                            CFileManager profile;
                                            CBinary binControlModel;

                                            binSearchID += m_binSysID;
                                            strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                                            if (!strData.size())
                                                return ecRet = CErrorCode::EC_DATA;
                                            profile.InitManager(strData);
                                            strData = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ControModel_TXT);
                                            binControlModel = HexString2Binary(strData);
                                            m_ucSecSelItem = binControlModel[1];
                                            if (CErrorCode::EC_SUCCESS == NewAppLayer(Subaru_SEARCH_SYS))
                                            {
                                                if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                                {
                                                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                                }
                                                else
                                                {
                                                    W_I8    iSysNum = 0;
                                                    CBinary bin;

                                                    m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                                    for (i = 0; i < m_bgSysId.GetByteCount(); i++)
                                                    {
                                                        iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                                    }
                                                    m_binSysID.Clear();
                                                    if (iSysNum == 1)
                                                    {
                                                        for (int k = 0; k < 5; k++)
                                                            bin += m_bgSysId[0][k];
                                                        m_binSysID = bin;
                                                        if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                                        {
                                                            if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                                            {
                                                                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                                FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                        }
                    }
                    else
                    {
                        ShowCommWaitInfo();
                        if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                        {
                            if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                            {
                                CBinary binSearchID(Subaru_ID, 1);
                                string  strData;
                                CFileManager profile;
                                CBinary binControlModel;

                                binSearchID += m_binSysID;
                                strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                                if (!strData.size())
                                    return ecRet = CErrorCode::EC_DATA;
                                profile.InitManager(strData);
                                strData = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ControModel_TXT);
                                binControlModel = HexString2Binary(strData);
                                m_ucSecSelItem = binControlModel[1];
                                if (CErrorCode::EC_SUCCESS == NewAppLayer(Subaru_SEARCH_SYS))
                                {
                                    if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance(ALM_SCAN_SYSTEM))
                                    {
                                        FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                    }
                                    else
                                    {
                                        W_I8    iSysNum = 0;
                                        CBinary bin;

                                        m_bgSysId = ((CSubaruBaseAllSystemApp*)m_pDiagnosisAppLayer)->GetSearchedSysId();
                                        for (i = 0; i < m_bgSysId.GetByteCount(); i++)
                                        {
                                            iSysNum += (m_bgSysId[i].GetByteCount() / 5);
                                        }
                                        m_binSysID.Clear();
                                        if (iSysNum == 1)
                                        {
                                            for (int k = 0; k < 5; k++)
                                                bin += m_bgSysId[0][k];
                                            m_binSysID = bin;
                                            if (CErrorCode::EC_SUCCESS == NewAppLayer(m_binSysID[0]))
                                            {
                                                if (CErrorCode::EC_SUCCESS != m_pDiagnosisAppLayer->Entrance())
                                                {
                                                    FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_ENTERSYSTEMFAIL, DF_MB_OK, DT_LEFT);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ecRet;
}