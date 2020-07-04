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

CBinaryGroup Ecu_Id_Select;  // ���ɸѡ���ECUID
string Country_Ecu_File;  // ��Ӧ��ECU�ļ�
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
      ��    �ܣ����������
      ��    ������
      �� �� ֵ���������
      ˵    ������
-----------------------------------------------------------------------------*/
//W_ErrorCode CSubaru::MainEntrance()
//{    
//    W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
//
//    //W_ErrorCode m_value = ChooseVehicle(); // ����
//    //if (m_value == 0)
//    //{
//    //    return ecRet;
//    //}
//
//    W_I16            iSelMenuItem = 0;
//    W_I16            iSelected = 0;
//    CMenuCtrl        mnuMenu, ctrl;    // ѡ��˵�
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
//        if (iSelected == 0)     // ��������
//        {
//#ifndef __NT5600DEF__
//            while (1)
//            {
//                mnuMenu.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x35",6));
//                mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x60",6));//���
//                mnuMenu.AddOneItem(CBinary("\xFF\x07\x00\x00\x00\x40",6));//�������⹦��
//                mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x63",6));//���
//                iSelMenuItem = mnuMenu.ShowCtrl();  
//                if (iSelMenuItem == -1)
//                    break;
//                switch(iSelMenuItem)
//                {
//                case 0x00:            // ���
//                    m_bIsService = false;
//                    DiagSoftware();    
//                    break;
//                case 0x01:            // �������⹦��
//                    m_bIsService = true;
//                    //Service();//
                //HotFunction();
//                    m_bIsService = false;
//                    break;
//                case 0x02:
//                    m_bIsService = true;
//                    isProgramming = true;
//                    Programming();    // ���
//                    m_bIsService = false;
//                    isProgramming = false;
//                    break;
//                }
//            }
//#else
//            DiagSoftware();
//#endif
//        }
//        if (iSelected == 1)   // BRZ���͵ļ��
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
    W_I16            Message_ME_MA = 0; // ������Ϣ
    CMenuCtrl        mnuMenu, ctrl, menuMA, menuME, vehiclemenu, vehicleyear;
    CBinary            binMenu("\x01\x02\x04", 3);
    // ���ң�
    CBinary JP = CBinary(Subaru_Country"\x01", 6);  // �ձ�
    CBinary NA = CBinary(Subaru_Country"\x02", 6);  // ����
    CBinary EU = CBinary(Subaru_Country"\x03", 6);  // ŷ��
    CBinary ASIA = CBinary(Subaru_Country"\x04", 6);  // ����/������
    CBinary ME = CBinary(Subaru_Country"\x05", 6);  // �ж�
    CBinary AF = CBinary(Subaru_Country"\x06", 6);  // ����
    CBinary MA = CBinary(Subaru_Country"\x07", 6); // �����޺�������

    // �ж���
    CBinary BL = CBinary(Subaru_ME_MA_Country"\x0D", 6); // ��������
    CBinary SP = CBinary(Subaru_ME_MA_Country"\x0E", 6); // ����·˹���͹�
    CBinary AL = CBinary(Subaru_ME_MA_Country"\x0F", 6); // �������������͹�
    CBinary YS = CBinary(Subaru_ME_MA_Country"\x10", 6); // ��ɫ�й�
    CBinary YD = CBinary(Subaru_ME_MA_Country"\x11", 6); // Լ����ϣķ����
    CBinary KW = CBinary(Subaru_ME_MA_Country"\x12", 6); // �����ع�
    CBinary LB = CBinary(Subaru_ME_MA_Country"\x13", 6); // ����۹��͹�
    CBinary ML = CBinary(Subaru_ME_MA_Country"\x14", 6); // Ħ�������
    CBinary AM = CBinary(Subaru_ME_MA_Country"\x15", 6); // �����յ���
    CBinary KT = CBinary(Subaru_ME_MA_Country"\x16", 6); // ��������
    CBinary ST = CBinary(Subaru_ME_MA_Country"\x17", 6); // ɳ�ذ���������
    CBinary TE = CBinary(Subaru_ME_MA_Country"\x18", 6); // �����乲�͹�
    CBinary QZ = CBinary(Subaru_ME_MA_Country"\x19", 6); // ����������������
    CBinary QT_E = CBinary(Subaru_ME_MA_Country"\x1A", 6); // ����

    // �����޺������ޣ�
    CBinary AT = CBinary(Subaru_ME_MA_Country"\x01", 6); // ����ϺͰͲ���
    CBinary BB = CBinary(Subaru_ME_MA_Country"\x02", 6); // �ͰͶ�˹
    CBinary BM = CBinary(Subaru_ME_MA_Country"\x03", 6); // ��Ľ��
    CBinary KM = CBinary(Subaru_ME_MA_Country"\x04", 6); // ����Ⱥ��
    CBinary DM = CBinary(Subaru_ME_MA_Country"\x05", 6); // �����������
    CBinary GL = CBinary(Subaru_ME_MA_Country"\x06", 6); // �����ɴ�
    CBinary YM = CBinary(Subaru_ME_MA_Country"\x07", 6); // �����
    CBinary SJ = CBinary(Subaru_ME_MA_Country"\x08", 6); // ʥ����
    CBinary SL = CBinary(Subaru_ME_MA_Country"\x09", 6); // ʥ¬����
    CBinary LN = CBinary(Subaru_ME_MA_Country"\x0A", 6); // ������
    CBinary TL = CBinary(Subaru_ME_MA_Country"\x0B", 6); // �������Ͷ�͸�
    CBinary QT_A = CBinary(Subaru_ME_MA_Country"\x0C", 6); // ����

    CBinary binSearchID = CBinary("\x00\x00\x00\x00\x00\x01", 6);  // ����ID����
    Vehcilename = CBinary("\x53\x5D", 2);
    //CBinary Vehcilename = CBinary("\x53\x5D",2);   // ��������ǰ׺
    CBinary binMainMenu, m_VehicleIndex;

    m_country.Clear();   // ��ձ����ظ�
    //CBinaryGroup m_country,country_ME,country_MA,m_vehicle,m_vehicle_name,m_vehicle_type,m_vehicle_year;
    // ���ң�
    m_country.Append(JP);
    m_country.Append(NA);
    m_country.Append(EU);
    m_country.Append(ASIA);
    m_country.Append(ME);
    m_country.Append(AF);
    m_country.Append(MA);

    // �ж���
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

    // �����޺�������:
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
        if (isbreak == false)  // �����߼�����
        {
            MainEntrance();
            break;
        }

        ctrl.InitCtrl(CBinary("\x53\xFF\x00\x03\x00\x00", 6));   // ����
        for (int i = 0; i < m_country.GetByteCount(); i++)
        {
            ctrl.AddOneItem(m_country[i]);
        }

        iSelected = ctrl.ShowCtrl();     // ������ʾ
        if (iSelected == -1)
        {
            if (FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_EXITSYSTEM, DF_MB_YESNO) == DF_IDYES)
                break;
            else
                continue;
        }
        else
        {
            Message_Country = iSelected;    // ѡ�е�����복����Ϣֵ��
        }

        if (m_country[iSelected] == JP)   // ����CBinary��ֵ�Զ�����ļ�����:   �ձ�/����/ŷ��....................
        {
            Country_Vin_File = SubaruVin_JP_CBF;
        }
        else if (m_country[iSelected] == NA)  // ����
        {
            Country_Vin_File = SubaruVin_NA_CBF;
        }
        else if (m_country[iSelected] == EU)   // ŷ��
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == ASIA)   // ����/������
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == AF)   // ����
        {
            Country_Vin_File = SubaruVin_EU_CBF;
        }
        else if (m_country[iSelected] == ME)  // �ж�
        {
        SHOWME:
            Country_Vin_File = SubaruVin_ME_CBF;

            while (1)
            {
                menuME.InitCtrl(CBinary("\x53\xFF\x00\x05\x00\x00", 6)); // ����

                for (int i = 0; i < country_ME.GetByteCount(); i++)
                {
                    menuME.AddOneItem(country_ME[i]);
                }

                ME_selcet = menuME.ShowCtrl();
                if (ME_selcet == -1)
                {
                    isbreak = false;  // �������߼�
                    break;
                }
                else
                {
                    Message_ME_MA = ME_selcet;  // ѡ�е�����복����Ϣֵ��
                    isbreak = true;   //  != -1 ������
                    break;
                }
            }
            if (isbreak == false)
            {
                MainEntrance();
                break;
            }
        }
        else if (m_country[iSelected] == MA)  // �����޺�������
        {
        SHOWMA:
            Country_Vin_File = SubaruVin_CSA_CBF;

            while (1)
            {
                menuMA.InitCtrl(CBinary("\x53\xFF\x00\x05\x00\x00", 6)); // ����

                for (int i = 0; i < country_MA.GetByteCount(); i++)
                {
                    menuMA.AddOneItem(country_MA[i]);
                }

                MA_select = menuMA.ShowCtrl();
                if (MA_select == -1)
                {
                    country_MA.Clear();   // ��ձ����ظ�
                    isbreak = false;  // �������߼�
                    break;
                }
                else
                {
                    Message_ME_MA = MA_select;  // ѡ�е�����복����Ϣֵ��
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
        //    strData = GetStringValue(Country_File,binMainMenu[i]);  // ��ȡID
        //    if(!strData.size())
        //        return ecRet = CErrorCode::EC_DATA;
        //    profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
        //    strMenu = profile.GetItemValue(Subaru_VIN_INDEX_TTL,Subaru_VIN_VehicleName_TXT);  
        //    strMenu += Subaru_Vehicle_Name;
        //    binMainMenu[i] = String2Binary(strMenu);
        //    m_vehicle.Append(binMainMenu[i]);
        //}

    SHOWEMPTY:
        while (1)
        {
            // ��գ������ظ�
            strData.clear();
            strMenu.clear();
            vehicle_name.clear();
            m_vehicle.Clear();
            m_vehicle_name.Clear();

            strData = GetStringValue(Country_Vin_File, binSearchID);  // ��ȡID
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_INDEX_TTL, Subaru_VIN_VehicleIndex_TXT);  // ��ȡINDEX��VehicleIndex��ֵ

            iNum = SeparatorString(strMenu, ",", vehicle_name);   // String2Binary����HexString2Binary����ʹ��string[i]����ֻ���ǵ����ַ������ô˺��������봦��
            for (int i = 0; i < iNum; i++)
            {
                binMainMenu = HexString2Binary(vehicle_name[i]);    // string -> CBinary ����
                m_vehicle.Append(binMainMenu);    // ���Ͳ���CBinarygroup��
            }

            for (int i = 0; i < m_vehicle.GetByteCount(); i++)   // ����CBinarygroup
            {
                binMainMenu.Clear();  // ÿ������һ����binMainMenu���������
                strData = GetStringValue(Country_Vin_File, m_vehicle[i]);  // ����ÿһ��͵�����
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
                strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETTL, Subaru_VIN_VehicleName_TXT);    // ��ȡ�������Ƶ��ַ���
                binMainMenu = String2Binary(strMenu);
                Vehcilename += binMainMenu;   // ���ַ���ǰ����\x53\x5D
                m_vehicle_name.Append(Vehcilename);    // ��ת����ĳ������Ʋ����µ�CBinarygroup
                Vehcilename = CBinary("\x53\x5D", 2);   // ÿ������һ�����Vehcilename���������
            }



            vehiclemenu.InitCtrl(CBinary("\x53\x60\xFF\x00\x00\x44", 6));
            for (int i = 0; i < m_vehicle_name.GetByteCount(); i++)   // ������������
            {
                vehiclemenu.AddOneItem(m_vehicle_name[i]);      // ����˵���
            }

            iSelected = vehiclemenu.ShowCtrl();   // ��ʾ����
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

                isbreak = false;   // �������߼�
                break;
            }
            else
            {
                Message_Vehicle = iSelected;  // ѡ�е�����복����Ϣֵ��
            }

            m_vehicle_type.Clear();   // ��գ������ظ�

            strData.clear();
            strMenu.clear();
            binMainMenu.Clear();
            strData = GetStringValue(Country_Vin_File, m_vehicle[iSelected]);  // ���ݵ��ѡȡVehicleIndex
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETTL, Subaru_VIN_VehicleType_TXT);    // ��ȡ������ݵ�����
            //binMainMenu = HexString2Binary(strMenu);
            //m_vehicle_type.Append(binMainMenu);    

            iNum2 = SeparatorString(strMenu, ",", vehicle_year);   // String2Binary����HexString2Binary����ʹ��string[i]����ֻ���ǵ����ַ������ô˺��������봦��
            for (int i = 0; i < iNum2; i++)
            {
                binMainMenu = HexString2Binary(vehicle_year[i]);    // �����������
                CBinary temp = CBinary("\x00", 1);
                temp += binMainMenu;
                m_vehicle_type.Append(temp);
            }

            //for (int i = 0;i < binMainMenu.GetByteCount();i++)
            //{
            //    strData = GetStringValue(Country_File,binMainMenu[i]);  // ��ȡID
            //    if(!strData.size())
            //        return ecRet = CErrorCode::EC_DATA;
            //    profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
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
                profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
                strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_Year_TXT);    // ��ȡ������ݵ�����
                //string TempStr  = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL,"SystemName");    // ��ȡ������ݵ�����
                binMainMenu = String2Binary(strMenu);
                Vehcilename += binMainMenu;
                bool isright = true;
                for (int k = 0; k < m_vehicle_year.GetByteCount(); k++)    // ȥ�����һ���Ĳ˵���
                {
                    if (m_vehicle_year[k].Compare(Vehcilename) == 0)
                    {
                        isright = false;
                    }
                }

                if (isright && Vehcilename != CBinary("\x53\x5d\x00\x00\x00\x2b", 6) && Vehcilename != CBinary("\x53\x5d", 2))
                {                                                // ���SystemNameΪ0002b����Ϊ�յĻ���ֱ�������ϴ���Ŀ��Ƶ�Ԫ����show���
                    m_vehicle_year.Append(Vehcilename);
                }
                else if (m_vehicle_year.GetByteCount() == 0)   // SystemName ���Ϊ0002b��Ϊ�յĻ����������Ϳ϶�������,Ϊ�˱������
                {
                    Vehcilename = CBinary("\x53\x5D", 2);
                    DiagSoftware();  // �ϴ�����ʾϵͳ
                    goto SHOWEMPTY;   // ���س���
                }

                Vehcilename = CBinary("\x53\x5D", 2);
            }

            while (1)
            {
                vehicleyear.InitCtrl(CBinary("\x53\xFF\x00\x02\x00\x00", 6));  // �ͺ�
                for (int i = 0; i < m_vehicle_year.GetByteCount(); i++)   // ������������
                {
                    vehicleyear.AddOneItem(m_vehicle_year[i]);      // ����˵���
                }

                iSelected = vehicleyear.ShowCtrl();   // ��ʾ����/���
                if (iSelected == -1)
                {
                    break;
                }
                else if (iSelected == m_vehicle_year.GetByteCount() - 1)  // ÿ��ݵ����һ����������ݣ����õ�����ϵͳ
                {
                    DiagSoftware();  // �ϴ�����ʾϵͳ
                    goto SHOWEMPTY;   // ���س���
                }
                else
                {
                Vechilemessage:
                    Message_Year = iSelected;  // ѡ�е�����복����Ϣֵ��

                    if (isshowmessage == true)  // ��������Ϣ�ķ���״̬
                    {
                        CEcuInfoCtrl Menumessage;
                        Menumessage.AddOneItem(CBinary("\x53\xFF\x00\x03\x00\x00", 6), FxGetStdString(m_country[Message_Country]));
                        Menumessage.AddOneItem(CBinary("\x53\x60\xFF\x00\x00\x44", 6), FxGetStdString(m_vehicle_name[Message_Vehicle]));
                        Menumessage.AddOneItem(CBinary("\x53\xFF\x00\x01\x00\x00", 6), FxGetStdString(m_vehicle_year[Message_Year]));
                        //Menumessage.AddOneItem(FxGetStdString(m_country[Message_Country]),FxGetStdString(m_country[Message_Country]));

                        while (1)
                        {
                            Message_select = Menumessage.ShowCtrl();   // ������Ϣ
                            if (Message_select == -1)
                            {
                                isshowmessage = false;    // ������Ϣ����״̬
                                break;
                            }
                        }
                    }


                    while (1)
                    {
                        mnuMenu.InitCtrl(CBinary("\xFF\x07\x00\x00\x00\x35", 6));
                        mnuMenu.AddOneItem(CBinary("\xFF\x05\x00\x00\x00\x60", 6));// ���
                        mnuMenu.AddOneItem(CBinary("\xFF\x07\x00\x00\x00\x40", 6));// �������⹦��
                        mnuMenu.AddOneItem(CBinary("\x01\x00\x00\x00\x01\x1B", 6));// ������Ϣ
                        iSelMenuItem = mnuMenu.ShowCtrl();
                        if (iSelMenuItem == -1)
                        {
                            break;
                        }
                        switch (iSelMenuItem)
                        {
                        case 0x00:            // ���
                            m_bIsService = false;
                            //DiagSoftware();    
                            Subaru_DiagSoftware();
                            break;
                        case 0x01:            // �������⹦��
                            m_bIsService = true;
                            //Service();//
                            HotFunction();
                            m_bIsService = false;
                            break;
                        case 0x02:
                            //m_bIsService = true;
                            //isProgramming = true;
                            //Programming();    // ���
                            //m_bIsService = false;
                            //isProgramming = false;
                            isshowmessage = true;
                            goto Vechilemessage;   // ������Ϣ
                            break;
                        }
                    }
                }
            }

        }
        continue;
    }


    //if (iSelected == 1)   // BRZ���͵ļ��
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
    vctcountry.clear();    //����
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
        for (k = 0; k < vctcountry.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
        {
            if (SearchResult[i][1] == vctcountry[k])
                break;
        }
        if (k == vctcountry.size())
            vctcountry.push_back(SearchResult[i][1]);
    }

    while (1)
    {
        uiMenu.InitCtrl(CBinary("\x53\xFF\x00\x03\x00\x00", 6));   // ����
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
        else if (iSelMenuItem == 0)  // ����
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
        else if (iSelMenuItem == 1) // �ձ�
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
        else if (iSelMenuItem == 2)  // ŷ��
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
        else if (iSelMenuItem == 3)  // ����
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


        // ����
        for (i = 0; i < SearchResultVehicle.size(); i++)
        {
            for (k = 0; k < vctStrVehicle.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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


        // ���
        vctYear.clear();

        for (int i = 0; i < SearchResultVehicle.size(); i++)
        {
            if (SearchResultVehicle[i][1] == vctStrVehicle[iSelMenuItem])
            {
                for (k = 0; k < vctYear.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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


        //else if (iSelMenuItem == 0)  // ��������
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
        //        for (k = 0; k < vctStrVehicleNA.size(); k ++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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

        //    // �����������
        //    vctYearNA.clear();
        //        
        //    for (int i = 0; i < SearchResultNA.size(); i++)
        //    {
        //        if (SearchResultNA[i][1] == vctStrVehicleNA[iSelMenuItem])
        //        {
        //            for (k = 0; k < vctYearNA.size(); k ++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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
        //else if (iSelMenuItem == 1)  // �ձ�����
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









    //vctStrYearMenu.clear();   // ���
    //SearchStrSubMenu.clear();
    //for (i = 0; i < SearchResult.size(); i ++)
    //{        
    //    if (SearchResult[i][1] == vctStrVehicleMenu[iSelMenuItem])
    //    {
    //        SearchStrSubMenu.push_back(SearchResult[i]);
    //        for (k = 0; k < vctStrYearMenu.size(); k ++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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
    CVehicleSave    VehicleSave;//����ĳ���
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
        mnuMenu.AddOneItem(CBinary("\x01\x00\x00\x00\x01\x0D", 6));  // �Զ�ɨ��
        mnuMenu.AddOneItem(CBinary("\x53\x00\x00\x00\x00\x02", 6)); // ���Ƶ�Ԫ
        iSelMenuItem = mnuMenu.ShowCtrl();
        if (iSelMenuItem == -1)
        {
            break;
        }
        else
        {
            vechile_system_name.clear();
            m_system_name.Clear();
            strData = GetStringValue(Country_Vin_File, m_vehicle_type[iSelected]);  // ����ÿһ��͵�����
            if (!strData.size())
                return ecRet = CErrorCode::EC_DATA;
            profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
            strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_SystemName_TXT);  // ��ȡ����ϵͳ����

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
            iSelMenuItem = Menu_system_name.ShowCtrl();    // ��ʾϵͳ����
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
        profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
        strMenu = profile.GetItemValue(Subaru_VIN_VEHICLETYPE_TTL, Subaru_VIN_ECUID);

        SeparatorString(strMenu, ",", Vin_Ecu_Id);

        SeparatorString(Vin_Ecu_Id[iSelMenuItem], "&", Vin_Ecu_Id_Select);   // ���ݵ�����Ӧ��ECUID��

        //Find_New_SeparatorString(strMenu,",","&",Vin_Ecu_Id);  // ���ECUID  ����","  ����"&"

        for (int i = 0; i < Vin_Ecu_Id_Select.size(); i++)
        {
            if (i == 0)    // ȡÿ��ĵ�һ��
            {
                System_Search_Id = Vin_Ecu_Id_Select[i];  // ��������������ı�ţ�string����
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
            ecRet = New_SubaCANProInto(binSearchID);  // ������

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
        //    if (FALSE == SubaAct.OpenTabFile(Suba_DTC_Dex_CBF))  //����������ͷ�ʽ
        //        return CErrorCode::EC_ECU_REFUSE;

        //    vsystem_mod.push_back("00000ac7");
        //    SubaDtc.SearchString(SearchReasault, FALSE, 1, 1, vsystem_mod);   


        //}

        //strData = GetStringValue(Country_Ecu_File,Ecu_Id_All[i]);
        //if(!strData.size())
        //    return ecRet = CErrorCode::EC_DATA;
        //    break;
        //profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
        //strMenu = profile.GetItemValue(Subaru_DTC_TTL,Subaru_BytePosDtc_TXT);  
        //if (strMenu == "1")
        //{
        //    Ecu_Id_Select.Append(Ecu_Id_All[i]);
        //    temp.push_back(strMenu);
        //}



            //if (Vin_Ecu_Id[i] == "00000ac7")   // TCM������
            //{
            //    if (FALSE == SubaAct.OpenTabFile(Suba_DTC_Dex_CBF))  //����������ͷ�ʽ
            //        return CErrorCode::EC_ECU_REFUSE;

            //    vsystem_mod.push_back("00000ac7");
            //    SubaAct.SearchString(SearchReasault, FALSE, 1, 1, vsystem_mod);   

            //    for (int j = 0; j < SearchReasault.size(); j++)
            //    {
            //        for (int k = 0; k < SearchReasault.size(); k++)
            //        {
            //            // ������ʱ��������ƴ��ֵ
            //            string acttemp = SearchReasault[j][k+1] + SearchReasault[j][k+2] + SearchReasault[j][k+3];   // ƴ��Suba_DTC_Dex [1] [2] [3]
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
        //    profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
        //    strMenu = profile.GetItemValue(Subaru_DTC_TTL,Subaru_DtcText_TXT);  
        //    CBinary menubin = HexString2Binary(strMenu);
        //    Vehcilename = CBinary("\x53\x5D",2);
        //    Vehcilename += menubin;
        //    

        //    Menu_Dtc.AddOneItem(Vehcilename);
        //}
        //Menu_Dtc.ShowCtrl();




        Ecu_Id_Select.Clear();  // ��գ��������
        strMenu.clear();
        Vin_Ecu_Id.clear();
        SearchReasault.clear();
        vsystem_mod.clear();
        Dtc_Ecu_Id.Clear();
    }










    VechileStatus = 1;  // ��������
    strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);  // ��ȡID
    if (!strData.size())
        return ecRet = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
    strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);  // ��ȡMenu��MenuItem��ֵ
    binMainMenu = String2Binary(strMenu);
    if (VehicleSave.GetCount())
        binMainMenu += VehicleRecord;
    while (1)
    {
        iSelMenuItem = 1;
#if defined(__NT800__) || defined(__NT5600ALLSYS__)
        iSelMenuItem = FxShowMenu(Subaru_TTL_MNU_MAIN, binMainMenu, mnuMenu);    //�Զ�ɨ�� �� ��ϵͳ���� �˵�
#endif
        if (-1 == iSelMenuItem)
        {
            break;
        }
        if (iSelMenuItem >= 0)
        {
            m_ucSelectItem = binMainMenu[iSelMenuItem];  // MenItem 1,2
            if (m_ucSelectItem == System_Unit)    //��ϵͳ���� �˵�
            {
                binSearchID.SetByteAt(4, m_ucSelectItem);  // 0x53,0x00,0x00,0x00,0x02,0x00    
                binSubSearchId.SetByteAt(4, m_ucSelectItem);//Kas
                strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // MenuItem��ֵ
                strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
                CBinary binSubMenu;
                CMenuCtrl mUnitMenu;
                CMenuCtrl    mMenuSub;
                CMenuCtrl    mMenu;
                //CBinary binChild;
                binSubMenu = String2Binary(strMenu);
                binSubMenu += CBinary("\xff", 1);   //��� OBDϵͳ �˵�

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
                        iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN, binSubMenu, mUnitMenu);  // ���Ƶ�Ԫ�˵�����ʾ
                    if (-1 == iSecSelItem)
                        break;

                    /* �ȳ���CAN */
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                    {
                        CBinary PtlCan;
                        binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);  // // 0x53,0x00,0x00,0x00,0x00,0x00    
                        binTemp.SetByteAt(5, binSubMenu[iSecSelItem]);   // ��5�����Ƶ�ԪMenuItem[���λ��]��
                        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                        if (strTemp.length() > 0)
                        {
                            profile.InitManager(strTemp);
                            PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);  // ȡ��Ӧ��ItemCanЭ��
                        }
                        if (PtlCan.GetByteCount() > 0)
                        {
                            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                            binTemp = Subaru_CANID_BASE;   //  0x53,0x4d,0x00,0x00,0x00,0x00    
                            binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);  // ��5��ȡ��Ӧ��ItemCan�����һ�

                            ecRet = SubaCANProInto(binTemp);  // ������
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
                                m_ucSecSelItem = binSubMenu[iSecSelItem];//KAS���������ͬ

                                // xxm 2016-08-11 ����ʵ���޸�
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
                                if (!strSubMenu.size())//�������ѭ�������Ӳ˵������˳�ѭ��
                                    break;
                            }
                        }
                    }
                    else if (binSubMenu[iSecSelItem] == 0xff)//OBDϵͳ
                    {
                        W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
                        CEobdAllSystem    EobdAllSystem;
                        W_U8            i;

                        for (i = CEobdAllSystem::PROTOCOL_BEGIN + 1; i < CEobdAllSystem::PROTOCOL_KWP; i++) //OBDϵͳֻ��CANЭ��
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
                    else//��Keyless access system��
                    {
                        m_ucSecSelItem = binSubMenu[iSecSelItem];
                        if (iSecSelItem >= 0)
                        {
                            // xxm 2016-08-11 ����ʵ���޸�
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
                                            // ������Э��û�н���ɹ�,����ɨ����һ��Э��,ֱ��SysNumTemp���ڵ��ڿ�ɨ���Э���������ɹ�
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
            else if (m_ucSelectItem == VehicleRecord)  // �Զ�ɨ��
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
    CVehicleSave    VehicleSave;//����ĳ���
    string            strMenu, strData, strTemp;
    W_I16            iSelMenuItem = 0;

    VechileStatus = 1;  // ��������
    strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);  // ��ȡID
    if (!strData.size())
        return ecRet = CErrorCode::EC_DATA;
    profile.InitManager(strData);  // ��ȡ��ֵ   SetItemValue
    strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);  // ��ȡMenu��MenuItem��ֵ
    binMainMenu = String2Binary(strMenu);
    if (VehicleSave.GetCount())
        binMainMenu += VehicleRecord;
    while (1)
    {
        iSelMenuItem = 1;
#if defined(__NT800__) || defined(__NT5600ALLSYS__)
        iSelMenuItem = FxShowMenu(Subaru_TTL_MNU_MAIN, binMainMenu, mnuMenu);    //�Զ�ɨ�� �� ��ϵͳ���� �˵�
#endif
        if (-1 == iSelMenuItem)
        {
            break;
        }
        if (iSelMenuItem >= 0)
        {
            m_ucSelectItem = binMainMenu[iSelMenuItem];  // MenItem 1,2
            if (m_ucSelectItem == System_Unit)    //��ϵͳ���� �˵�
            {
                binSearchID.SetByteAt(4, m_ucSelectItem);  // 0x53,0x00,0x00,0x00,0x02,0x00    
                binSubSearchId.SetByteAt(4, m_ucSelectItem);//Kas
                strData = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
                if (!strData.size())
                    return ecRet = CErrorCode::EC_DATA;
                profile.InitManager(strData);  // MenuItem��ֵ
                strMenu = profile.GetItemValue(Subaru_INI_Menu_TTL, Subaru_INI_MenuItem_TXT);
                CBinary binSubMenu;
                CMenuCtrl mUnitMenu;
                CMenuCtrl    mMenuSub;
                CMenuCtrl    mMenu;
                //CBinary binChild;
                binSubMenu = String2Binary(strMenu);
                binSubMenu += CBinary("\xff", 1);   //��� OBDϵͳ �˵�

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
                        iSecSelItem = FxShowMenu(Subaru_TTL_MNU_SUBMAIN, binSubMenu, mUnitMenu);  // ���Ƶ�Ԫ�˵�����ʾ
                    if (-1 == iSecSelItem)
                        break;

                    /* �ȳ���CAN */
                    if (CErrorCode::EC_SUCCESS == NewAppLayer(0x40))
                    {
                        CBinary PtlCan;
                        binTemp = CBinary(Subaru_ID"\x00\x00\x00\x00\x00", 6);  // // 0x53,0x00,0x00,0x00,0x00,0x00    
                        binTemp.SetByteAt(5, binSubMenu[iSecSelItem]);   // ��5�����Ƶ�ԪMenuItem[���λ��]��
                        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binTemp);
                        if (strTemp.length() > 0)
                        {
                            profile.InitManager(strTemp);
                            PtlCan = profile.GetBinary(Subaru_INI_Protocol_TTL, Subaru_INI_ItemCan_TXT);  // ȡ��Ӧ��ItemCanЭ��
                        }
                        if (PtlCan.GetByteCount() > 0)
                        {
                            FxShowMessageBox(STD_TTL_MSG_INFORMATION, STD_TXT_MSG_ESTABILISHCOMM, DF_MB_NOBUTTON, DT_CENTER);
                            binTemp = Subaru_CANID_BASE;   //  0x53,0x4d,0x00,0x00,0x00,0x00    
                            binTemp.SetByteAt(5, PtlCan[PtlCan.GetByteCount() - 1]);  // ��5��ȡ��Ӧ��ItemCan�����һ�

                            ecRet = SubaCANProInto(binTemp);  // ������
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
                                m_ucSecSelItem = binSubMenu[iSecSelItem];//KAS���������ͬ

                                // xxm 2016-08-11 ����ʵ���޸�
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
                                if (!strSubMenu.size())//�������ѭ�������Ӳ˵������˳�ѭ��
                                    break;
                            }
                        }
                    }
                    else if (binSubMenu[iSecSelItem] == 0xff)//OBDϵͳ
                    {
                        W_ErrorCode        ecRet = CErrorCode::EC_SUCCESS;
                        CEobdAllSystem    EobdAllSystem;
                        W_U8            i;

                        for (i = CEobdAllSystem::PROTOCOL_BEGIN + 1; i < CEobdAllSystem::PROTOCOL_KWP; i++) //OBDϵͳֻ��CANЭ��
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
                    else//��Keyless access system��
                    {
                        m_ucSecSelItem = binSubMenu[iSecSelItem];
                        if (iSecSelItem >= 0)
                        {
                            // xxm 2016-08-11 ����ʵ���޸�
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
                                            // ������Э��û�н���ɹ�,����ɨ����һ��Э��,ֱ��SysNumTemp���ڵ��ڿ�ɨ���Э���������ɹ�
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
            else if (m_ucSelectItem == VehicleRecord)  // �Զ�ɨ��
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
    profile.InitManager(strTemp);  // ��ȡ�ַ�
    strTemp = profile.GetItemValue(Subaru_ECU_TTL, Subaru_ECUAddr_TXT);    // ��ȡECUID
    binEcuId = HexString2Binary(strTemp);
    strTemp = profile.GetItemValue(Subaru_ECU_TTL, Subaru_ToolAddr_TXT);    // ��ȡTOOLID
    binToolId = HexString2Binary(strTemp);
    strTemp = profile.GetItemValue(Subaru_CMD_TTL, Subaru_EnterCmd_TXT);    // ��ȡENTERCMD
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
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = atoi(strBaudRate.c_str());   // ������
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = "6,14";   // ����
    for (int i = 0; i < vctStr.size(); i++)
    {
        ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));  // ʱ�����
    }

    rect = m_pDiagnosisAppLayer->New_Entrance(ALM_DIAGNOSE);
    if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag && CErrorCode::EC_SUCCESS == rect)
        return CErrorCode::EC_SUCCESS;
    else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->SysFlag)
        return  rect;

    //vector<string> vctStr;
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = profile.GetInt(Subaru_INI_Netlayer_TTL, Subaru_INI_BaudRate_TXT); // ȡNetlayer��BaudRate��ֵ  �����ڵ�Ͳ�����
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ComPin_TXT);  // ȡNetlayer��ComPin��ֵ  �����ڵ��ͨѶ����
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_NextID_TXT);  //  ȡNetlayer��NextID��ֵ
    ////strTemp = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_CommTime_TXT);  // ȡNetlayer��CommTime��ֵ  �����ڵ��ʱ�����
    //SeparatorString(strTemp, ",", vctStr);
    //for (W_I16 i = 0; i < vctStr.size(); i++)
    //    ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));

    //if (1 != VechileStatus && 2 != VechileStatus)
    //    return CErrorCode::EC_DATA;
    //if (FALSE == Search_NextID.OpenTabFile(Subaru_CANProInto_CBF))  //NEXTID��ѯ�ļ�->�豸ID,ECU ID,��������
    //    return CErrorCode::EC_DATA;
    //if (FALSE == Search_SubaDLLPrefix.OpenTabFile(Subaru_DLLPrefix_CBF))  //ϵͳ��Ų�ѯ
    //    return CErrorCode::EC_DATA;

    //if (1 == VechileStatus)  // ��������1,BRZ����2
    //    strTemp = "00000001";
    //else
    //    strTemp = "00000002";
    //vctTemp.push_back(strTemp);
    //Search_SubaDLLPrefix.SearchString(vctDLLprefix, FALSE, 0, 0, vctTemp);  // ����ϵͳ���
    //if (0 >= vctDLLprefix.size())
    //    return CErrorCode::EC_DATA;

    //vctTemp.clear();
    //vctTemp.push_back(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID);  // NextID
    //((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->VehicleStatus = VechileStatus;  // ����
    //Search_NextID.SearchString(vct_Resault, FALSE, 0, 0, vctTemp);   // SubaCANProInto ������NextID���� vct_Resault��
    //if (0 >= vct_Resault.size())
    //    return CErrorCode::EC_DATA;
    //for (W_I16 i = 0; i < vct_Resault.size(); i++)  // ����
    //{
    //    vctTemp = vct_Resault[i];
    //    if (vctTemp.size() != 7)
    //        continue;

    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID = String2Binary(vctTemp[5]);  // EcuID Ϊ�� SubaCANProInto ������ NextID �ĵ�5��
    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ToolID = String2Binary(vctTemp[4]); // ToolID Ϊ�� SubaCANProInto������ NextID �ĵ�4��
    //    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EnterCMD = String2Binary(vctTemp[6]);// EnterCMD Ϊ�� SubaCANProInto ������ NextID �ĵ�6��
    //    strTemp = vctTemp[3];  // strTemp Ϊ�� SubaCANProInto ������ NextID �ĵ�3��
    //    for (W_I16 j = 0; j < vctDLLprefix.size(); j++)
    //    {
    //        W_I16 IDLenth = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID.GetByteCount();  // IDLenth Ϊ EcuID �ĳ���
    //        if (vctDLLprefix[j].size() < 5)
    //            continue;
    //        binTemp = String2Binary(vctDLLprefix[j][1]);   // �� SubaDLLPrefix �ĵ�1��ת��Ϊ CBinary
    //        if (binTemp[0] == ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID[IDLenth - 1] && vctDLLprefix[j][3] == strTemp)  
    //        {  // binTemp[0] == EcuID�����һλ  && SubaDLLPrefix �еĵ�3�� == SubaCANProInto ������ NextID �ĵ�3��
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



W_ErrorCode CSubaru::SubaCANProInto(CBinary binSearchID)   // �Զ�ɨ��
{
    W_ErrorCode rect = CErrorCode::EC_DATA;

    string strTemp;
    CBinary binTemp;
    CSearchString Search_NextID;
    CSearchString Search_SubaDLLPrefix;
    vector<string> vctTemp;
    vector<vector<string>> vctDLLprefix;
    vector<vector<string>> vct_Resault;

    if (1 == VechileStatus)  // ����������binSearchID      0x53,0x4D,0x00,0x00,0x00,0x??
        strTemp = GetStringValue(Subaru_Protocol_Ini_File, binSearchID);
    else if (2 == VechileStatus)
        strTemp = GetStringValue(Subaru_Protocol_Ini_Brz_File, binSearchID);
    if (strTemp.length() <= 0)
        return CErrorCode::EC_DATA;

    CFileManager profile;
    profile.InitManager(strTemp);  // ��ȡ�ַ�
    vector<string> vctStr;
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->BaudRate = profile.GetInt(Subaru_INI_Netlayer_TTL, Subaru_INI_BaudRate_TXT); // ȡNetlayer��BaudRate��ֵ  �����ڵ�Ͳ�����
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ComPin = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_ComPin_TXT);  // ȡNetlayer��ComPin��ֵ  �����ڵ��ͨѶ����
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_NextID_TXT);  //  ȡNetlayer��NextID��ֵ
    strTemp = profile.GetItemValue(Subaru_INI_Netlayer_TTL, Subaru_INI_CommTime_TXT);  // ȡNetlayer��CommTime��ֵ  �����ڵ��ʱ�����
    SeparatorString(strTemp, ",", vctStr);
    for (W_I16 i = 0; i < vctStr.size(); i++)
        ((((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET)->CommTime).push_back(atoi(vctStr[i].c_str()));

    if (1 != VechileStatus && 2 != VechileStatus)
        return CErrorCode::EC_DATA;
    if (FALSE == Search_NextID.OpenTabFile(Subaru_CANProInto_CBF))  //NEXTID��ѯ�ļ�->�豸ID,ECU ID,��������
        return CErrorCode::EC_DATA;
    if (FALSE == Search_SubaDLLPrefix.OpenTabFile(Subaru_DLLPrefix_CBF))  //ϵͳ��Ų�ѯ
        return CErrorCode::EC_DATA;

    if (1 == VechileStatus)  // ��������1,BRZ����2
        strTemp = "00000001";
    else
        strTemp = "00000002";
    vctTemp.push_back(strTemp);
    Search_SubaDLLPrefix.SearchString(vctDLLprefix, FALSE, 0, 0, vctTemp);  // ����ϵͳ���
    if (0 >= vctDLLprefix.size())
        return CErrorCode::EC_DATA;

    vctTemp.clear();
    vctTemp.push_back(((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->NextID);  // NextID
    ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->VehicleStatus = VechileStatus;  // ����
    Search_NextID.SearchString(vct_Resault, FALSE, 0, 0, vctTemp);   // SubaCANProInto ������NextID���� vct_Resault��
    if (0 >= vct_Resault.size())
        return CErrorCode::EC_DATA;
    for (W_I16 i = 0; i < vct_Resault.size(); i++)  // ����
    {
        vctTemp = vct_Resault[i];
        if (vctTemp.size() != 7)
            continue;

        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID = String2Binary(vctTemp[5]);  // EcuID Ϊ�� SubaCANProInto ������ NextID �ĵ�5��
        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ToolID = String2Binary(vctTemp[4]); // ToolID Ϊ�� SubaCANProInto������ NextID �ĵ�4��
        ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EnterCMD = String2Binary(vctTemp[6]);// EnterCMD Ϊ�� SubaCANProInto ������ NextID �ĵ�6��
        strTemp = vctTemp[3];  // strTemp Ϊ�� SubaCANProInto ������ NextID �ĵ�3��
        for (W_I16 j = 0; j < vctDLLprefix.size(); j++)
        {
            W_I16 IDLenth = ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID.GetByteCount();  // IDLenth Ϊ EcuID �ĳ���
            if (vctDLLprefix[j].size() < 5)
                continue;
            binTemp = String2Binary(vctDLLprefix[j][1]);   // �� SubaDLLPrefix �ĵ�1��ת��Ϊ CBinary
            if (binTemp[0] == ((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->EcuID[IDLenth - 1] && vctDLLprefix[j][3] == strTemp)
            {  // binTemp[0] == EcuID�����һλ  && SubaDLLPrefix �еĵ�3�� == SubaCANProInto ������ NextID �ĵ�3��
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

/* BRZ���͵ļ�� �˵� */
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
        if (0 == iSelect)    // �Զ�ɨ��
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

                //OBDϵͳֻ��CANЭ��
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
      ��    �ܣ�ά������
      ��    ����iSequence ---- ϵͳ��ַ
      �� �� ֵ���������
      ˵    �����������⹦�ܺ���
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
                    if (strdata.size())            //ģʽ���ܶԲ���
                        profile.InitManager(strdata);
                    else
                        continue;
                }
                if (2 == j)
                {
                    ProgrammingMenuSet.Append(m_binSysID);
                    break;
                }

                strPart = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_Part_TXT);        //�������ܲ˵�
                strItem = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_Item_TXT);        //�������ܲ˵�
                strMenuCond = profile.GetItemValue(Subaru_INI_SpecialFunc_TTL, Subaru_INI_MenuCond_TXT);//�������ܲ˵�����ʾ����

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
                                    m_binThirdMenu += 0x04;            //����΢��������
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
                                        m_binThirdMenu += CBinary("\x03\x05", 2);            //������
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
                    if (iKey == 0)            //������
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
                    else if (iKey == 1)        //����Ӧ
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
                    else if (iKey == 2)        //����΢��������
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

// �������⹦��
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
            for (k = 0; k < vctStrMenu.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
            {
                if (SearchResult[i][1] == vctStrMenu[k])
                    break;
            }
            if (k == vctStrMenu.size())
                vctStrMenu.push_back(SearchResult[i][1]);
        }

        if (SearchResult[i][1] == "FF0700000049")  // �ƶ�����
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
        if (strTitleID == "FF070000004A")//�������������߻���
            vctStrPro.push_back("02");//KWP1
        if (strTitleID == "FF070000004D")//ת��
            vctStrPro.push_back("03");//KWP2

        vctStrPro.push_back("04");//CAN

        vctSelMenu.clear();
        if (strTitleID == "FF070000004D" || strTitleID == "FF0700000042")//ת��  �綯פ���ƶ�
            vctSelMenu.push_back("03");
        else
        {
            if (strTitleID == "FF0700000048")  // ���
            {
                vctSelMenu.push_back("17");
                //vctSelMenu.push_back("18");
            }

            if (strTitleID == "FF0700000055")  // ���
            {
                vctSelMenu.push_back("1A");
                //vctSelMenu.push_back("12");
            }

            if (strTitleID == "FF0700000052")  // ����
            {
                vctSelMenu.push_back("1F");
            }

            if (strTitleID == "FF0700000058")  // ̥ѹ���ϵͳ   Э��
            {
                vctSelMenu.push_back("0C");
            }

            if (strTitleID == "FF0700000051")  // ���ţ�����
            {
                vctSelMenu.push_back("2E");
                vctSelMenu.push_back("19");
            }

            vctSelMenu.push_back("01");
            if (strTitleID == "FF070000004B")//����ϵͳ��Կ��
                vctSelMenu.push_back("06");
        }

        vctStrFuncMenu.clear();
        vctSpecFuncID.clear();
        vctSysProInfo.clear();
        vctSysSelMenuInfo.clear();
        for (i = 0; i < vctStrPro.size(); i++)//CAN��ǰ ����Э���ں�
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
                Resetwithouttool();  //�ֶ�����
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

//�ֶ�����
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
    vctStrVehicleMenu.clear();    //����
    vctStr.clear();
    vctStr.push_back("532100000101");
    SubaOilReserMT.SearchString(SearchResult, FALSE, 0, 0, vctStr);
    for (i = 0; i < SearchResult.size(); i++)
    {
        for (k = 0; k < vctStrVehicleMenu.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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

        vctStrYearMenu.clear();   // ���
        SearchStrSubMenu.clear();
        for (i = 0; i < SearchResult.size(); i++)
        {
            if (SearchResult[i][1] == vctStrVehicleMenu[iSelMenuItem])
            {
                SearchStrSubMenu.push_back(SearchResult[i]);
                for (k = 0; k < vctStrYearMenu.size(); k++)    //��ȡ�������⹦�ܵĲ˵���ȥ�ظ�����
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





// ���
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
    for (W_I16 i = 0; i < ProgrammingMenuSet.GetByteCount(); i++)    // ��һ���˵�
    {
        bgTemp.Clear();
        if (CBinary("\xC2\x00\x00\x00\x40", 5) == ProgrammingMenuSet[i])
        {
            bgTemp.Append(STD_TTL_MNU_BODY);
            bgTemp.Append(ProgrammingMenuSet[i]);
        }
        else if (1 == 1)    // �����ӱ�̹��ܲ˵�ʱ�������
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
            for (W_I16 i = 1; i < bgTemp.GetByteCount(); i++)            // �������˵�
            {
                bgTemp2.Clear();
                if (bgTemp[i] == CBinary("\xC2\x00\x00\x00\x40", 5))    // ���ɵ�Ԫ->���ɵ�Ԫ����
                {
                    bgTemp2.Append(bgTemp[i]);                            // ϵͳID
                    bgTemp2.Append(CBinary("\x53\x00\x00\x00\x02\x0f", 6));
                    Menu2.AddOneItem(CBinary("\x53\x00\x00\x00\x02\x0f", 6));    // ֱ���ֶ����ĵ�SUBASPECFUNCINI.txt�в�ѯ���
                    Menu2IdSet.push_back(bgTemp2);
                }
                else if (1 == 1)    // �����ӱ�̹��ܲ˵�ʱ���������    ����MenuAndProcess���Switch����Ҳ��Ӧ��Ҫ����
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
      ��    �ܣ������µ�Ӧ�ò�
      ��    ����iSequence ---- ϵͳ��ַ
      �� �� ֵ���������
      ˵    ��������ϵͳ��ַ������Ӧ��Ӧ�ò�
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
      ��    �ܣ�ȡ��ϵͳ���
      ��    ����binaddr ---- ϵͳ��ַ
      �� �� ֵ������ϵͳ���
      ˵    ������
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
      ��    �ܣ��Զ�ɨ��
      ��    ������
      �� �� ֵ���������
      ˵    ����ʵ���Զ�ɨ��ȹ���
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
    bool            bIsKas = false;//kasϵͳ����
    bool            bKasErase = true;//Kas����ɹ���־
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
                        20090212 JIANRONG����
                        Ŀ�ģ�����ϵͳ���˳�ϵͳʱ�����¿��ٶ�ȡ��ϵͳ���ǲ��ǻ����ڹ����롣
                        */
                        scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST7);
                        scanselect.StartClearDtc();
                        scanselect.ShowCtrl();
                        if (CErrorCode::EC_SUCCESS == NewAppLayer(0x00))
                        {
                            //m_binSysID = binSysId;
                            W_ErrorCode    ecRet;
                            if (m_binSysID[0] == Kwp2_KAS)//KASϵͳ���⴦��
                            {
                                for (i = 0; i < m_vctkastoken.size(); i++)
                                {
                                    m_iKas = m_vctkastoken[i].iSysNum;//����KAS��ϵͳ��
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
                                        scanselect.SetHaveDtc(iRet, true);//�����й�����
                                        string strtemp;
                                        char   chvalue[10];
                                        strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                        sprintf(chvalue, "%d", CanCodeTemp.size());
                                        strtemp += chvalue;
                                        scanselect.SetItemValue(iRet, strtemp);
                                    }
                                    else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                                    {
                                        scanselect.SetHaveDtc(iRet, false);//�����޹�����
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
                                        scanselect.SetHaveDtc(iRet, false);//�����޹�����
                                        if ((m_token.ReadFlag) && (m_token.SysMode))//���룬�޹�����
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST2);
                                        else if ((!m_token.ReadFlag) && (m_token.SysMode))//���룬���ܶ���
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST4);
                                        else if (!m_token.SysMode)//���� �Ҳ���ģʽ
                                            scanselect.SetItemValue(iRet, Subaru_TXT_QUICKTEST5);
                                    }
                                    else if (m_token.iDtcNum != 0)
                                    {
                                        scanselect.SetHaveDtc(iRet, true);//�����޹�����
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
                        if (m_binSysID[0] == Kwp2_KAS)//���ڹ����룬������������򲻷���������.ֻҪ����һ����ϵͳ����ʧ�ܣ���ϵͳ������ʧ��
                        {
                            for (int j = 0; j < m_vctkastoken.size(); j++)
                            {
                                m_iKas = m_vctkastoken[j].iSysNum;//��ϵͳ��
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
                                scanselect.SetHaveDtc(i, true);//�����й�����
                                string strTemp;
                                char   chvalue[10];
                                strTemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                                sprintf(chvalue, "%d", CanCodeTemp.size());
                                scanselect.SetItemValue(i, strTemp + chvalue);
                            }
                            else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                            {
                                scanselect.SetHaveDtc(i, false);//�����޹�����
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
                                    scanselect.SetHaveDtc(i, false);//�����޹�����
                                    if ((m_token.ReadFlag) && (m_token.SysMode))//���룬�޹�����
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST2);
                                    else if ((!m_token.ReadFlag) && (m_token.SysMode))//���룬���ܶ���
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST4);
                                    else if (!m_token.SysMode)//���� �Ҳ���ģʽ
                                        scanselect.SetItemValue(i, Subaru_TXT_QUICKTEST5);
                                }
                                else if (m_token.iDtcNum != 0)
                                {
                                    scanselect.SetHaveDtc(i, true);//���ù�����
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
                if (m_binSysID[0] == Kwp2_KAS)//KASϵͳ���⴦��
                {
                    for (i = 1; i <= 3; i++)
                    {
                        m_iKas = i;//����KAS��ϵͳ��
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
                            scanselect.SetHaveDtc(iSysCount, true);//�����й�����
                            string strtemp;
                            char   chvalue[10];
                            strtemp = FxGetStdString(Subaru_TXT_QUICKTEST3);
                            sprintf(chvalue, "%d", CanCodeTemp.size());
                            strtemp += chvalue;
                            scanselect.SetItemValue(iSysCount, strtemp);
                        }
                        else if (((CSubaruBaseApp*)m_pDiagnosisAppLayer)->SubaruBRZ_SET->ReadFlag)
                        {
                            scanselect.SetHaveDtc(iSysCount, false);//�����޹�����
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
                    if (Kwp2_KAS == m_binSysID.GetByteAt(0))//KASϵͳ����
                    {
                        for (i = 0; i < m_vctkastoken.size(); i++)
                        {
                            m_kastoken = m_vctkastoken[i];
                            m_token.iDtcNum += m_kastoken.iDtcNum;//�õ�����������
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
                            scanselect.SetHaveDtc(iSysCount, false);//�����޹�����
                            if ((m_token.ReadFlag) && (m_token.SysMode))//���룬�޹�����
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST2);
                            else if ((!m_token.ReadFlag) && (m_token.SysMode))//���룬���ܶ���
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST4);
                            else if (!m_token.SysMode)//���� �Ҳ���ģʽ
                                scanselect.SetItemValue(iSysCount, Subaru_TXT_QUICKTEST5);
                        }
                        else//���룬�й�����
                        {
                            scanselect.SetHaveDtc(iSysCount, true);//�����й�����
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
      ��    �ܣ��õ��ַ���
      ��    ����strfilename ---- �ļ���
                bintempid   ---- �ı�ID
      �� �� ֵ���ַ���
      ˵    ���������ļ��е��ı�ID���õ���Ӧ���ַ���
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
            if (m_token.iMode == 0x01 && m_token.bgDtcRecv[1][2] != 0x00 && m_token.bgDtcRecv[1][2] != 0x01 && m_token.bgDtcRecv[1][2] != 0x02)//absģʽ1������״̬����
                troublecode.AddOneItem(m_token.bgDtcID[0], Subaru_MSG_Reference);
            else
                troublecode.AddOneItem(m_token.bgDtcID[0], Subaru_MSG_Latest);
        }
        if (m_token.bOld)
        {
            if (m_token.iMode == 0x01 && m_token.bgDtcRecv[1][2] != 0x00 && m_token.bgDtcRecv[1][2] != 0x01)//ȡ������״̬
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
      ��    �ܣ���ʾKWP1 ϵͳ������
      ��    ������
      �� �� ֵ����
      ˵    ������
-----------------------------------------------------------------------------*/
void CSubaru::DtcKwp1Show(CTroubleCodeCtrl& troublecode)
{
    W_I16 i;
    for (i = 0; i < m_token.bgDtcID.GetByteCount(); i++)
        troublecode.AddOneItem(m_token.bgDtcID[i], "");
}

/*-----------------------------------------------------------------------------
��    �ܣ���ʾCan ϵͳ������
��    ������
�� �� ֵ����
˵    ������
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
        binTemp = CBinary(STD_STANDARD"\x13", 6);    //��ο�����ά���ֲ�
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
��    �ܣ���ʾKWP2 ϵͳ������
��    ������
�� �� ֵ����
˵    ������
-----------------------------------------------------------------------------*/
void CSubaru::DtcKwp2Show(CTroubleCodeCtrl& troublecode)
{
    CBinary            binDtcID(Subaru_ID"\x00\x00\x00\x00\x00", 6);
    CBinary         binDtcStat;//������״̬

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
        for (int j = 0; j < iDtcLen; j++)//�����������1���ֽھ�������(ID)ֻ�ɵ�4���ֽ���ȷ��������
        {
            W_I16 iSize = 4 + j;
            if (iSize < 6)
                binDtcID.SetByteAt((4 + j), binDtcBuff[iDtcLen * i + j]);
        }
        if (iDtcStat)
        {
            iBin = binDtcBuff[iDtcLen * i + 2];//����������״ֵ̬
            if (iBin == 0xfe)//���Э��
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
      ��    �ܣ��Ƚ�ϵͳ���
      ��    ����binaddr ----ϵͳ��ַ
      �� �� ֵ������
      ˵    ������
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

void CSubaru::ShowCommWaitInfo()//��ʾͨѶ�Ⱥ���Ϣ
{
    string str1, str2, str3;
    str1 = FxGetStdString(Subaru_TTL_MNU_SEARCHID);
    str2 = FxGetStdString(STD_TXT_MSG_ESTABILISHCOMM);
    //str3 = FxGetStdString(Subaru_TXT_WAITFOR);
    //str2 += str3;
    FxShowMessageBox(str1, str2, DF_MB_NOBUTTON);
}
/*-----------------------------------------------------------------------------
      ��    �ܣ��õ�ϵͳ�����
      ��    ����binaddr ---- ϵͳ��ַ
      �� �� ֵ��ϵͳ���
      ˵    ������
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
    CVehicleSave    VehicleSave;//����ĳ���
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
                                m_iKas = i;//����KAS��ϵͳ��
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