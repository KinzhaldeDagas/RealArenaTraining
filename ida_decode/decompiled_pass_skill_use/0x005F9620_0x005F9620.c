// 0x005F9620 @ 0x005F9620
char __usercall sub_5F9620@<al>(TESObjectREFR *a1@<ecx>, double st6_0@<st1>, double a3@<st0>)
{
  TESObjectCELL *ParentCell; // eax
  ExtraDataList *v6; // edi
  int *v7; // ebx
  TESObjectREFRVtbl *vtbl; // ecx
  int *unk1F4; // edi
  int v10; // eax
  double v11; // st5
  int vtbl_high; // eax
  int *v13; // ebx
  int v14; // eax
  Data *v15; // edi
  int v16; // ecx
  int v17; // eax
  int v18; // ebx
  TESObjectREFRVtbl *v19; // eax
  int v20; // eax
  NiTransform *v21; // edi
  NiPoint3 *v22; // eax
  float *v23; // eax
  float v24; // ecx
  float v25; // edx
  float v26; // eax
  NiPoint3 *v27; // eax
  float *v28; // eax
  TESObjectCELL *(__thiscall *v29)(TESChildCELL *); // ecx
  TESForm *v30; // edx
  float v31; // eax
  TESObjectREFRVtbl *v32; // ecx
  int v33; // eax
  int v34; // eax
  double v35; // st7
  int v36; // eax
  int v37; // edi
  int v38; // eax
  int *v39; // eax
  int v40; // eax
  int *SafeFloatPointer; // eax
  float *v42; // ecx
  double v43; // st7
  double v44; // st6
  int *v45; // eax
  int *v46; // eax
  int v47; // edi
  double v48; // st7
  float *v49; // eax
  Data *data; // edi
  TESForm::FormFlags v51; // eax
  TESObjectCELL *v52; // eax
  int v53; // ebx
  TESObjectREFRVtbl *v54; // edi
  float *v55; // eax
  TESObjectCELL *v56; // eax
  int v57; // eax
  _DWORD *v58; // ebx
  TESObjectREFRVtbl *v59; // ecx
  int v60; // eax
  int v61; // edx
  int v62; // ebx
  int v63; // eax
  int **v64; // eax
  unsigned int *v65; // edi
  unsigned __int8 v66; // al
  int v67; // edx
  TESObjectCELL *v68; // eax
  int v69; // eax
  _DWORD *v70; // esi
  float v72; // [esp+Ch] [ebp-F0h]
  const char *flags; // [esp+14h] [ebp-E8h]
  TESObjectCELL *(__thiscall *GetChildCell)(TESChildCELL *); // [esp+18h] [ebp-E4h]
  TESForm *baseForm; // [esp+1Ch] [ebp-E0h]
  int x_low; // [esp+20h] [ebp-DCh]
  Data *v77; // [esp+20h] [ebp-DCh]
  float scale; // [esp+24h] [ebp-D8h]
  int v79; // [esp+24h] [ebp-D8h]
  int v80; // [esp+24h] [ebp-D8h]
  void *niNode; // [esp+28h] [ebp-D4h]
  TESObjectCELL *v82; // [esp+2Ch] [ebp-D0h]
  char v83; // [esp+2Ch] [ebp-D0h]
  float v84; // [esp+30h] [ebp-CCh]
  signed int v85; // [esp+30h] [ebp-CCh]
  char v86; // [esp+30h] [ebp-CCh]
  float FatigueFraction; // [esp+34h] [ebp-C8h]
  float v88; // [esp+38h] [ebp-C4h]
  double v89; // [esp+3Ch] [ebp-C0h]
  int v90; // [esp+44h] [ebp-B8h]
  float v91; // [esp+48h] [ebp-B4h]
  int v92; // [esp+4Ch] [ebp-B0h]
  float v93; // [esp+50h] [ebp-ACh]
  char v94; // [esp+53h] [ebp-A9h]
  float v95; // [esp+54h] [ebp-A8h] BYREF
  TESObjectREFR v96; // [esp+58h] [ebp-A4h] BYREF
  float v97[3]; // [esp+B4h] [ebp-48h] BYREF
  float a2[3]; // [esp+C0h] [ebp-3Ch] BYREF
  float v99[7]; // [esp+CCh] [ebp-30h] BYREF
  unsigned int v100; // [esp+F8h] [ebp-4h]
  int savedregs; // [esp+FCh] [ebp+0h] BYREF

  HIBYTE(v93) = 0; /*0x5f9662*/
  ParentCell = TESObjectREFR_GetParentCell(a1); /*0x5f9667*/
  v6 = (ExtraDataList *)ParentCell; /*0x5f966c*/
  if ( !ParentCell ) /*0x5f9670*/
  {
    v7 = 0; /*0x5f9695*/
    goto LABEL_6; /*0x5f9695*/
  }
  if ( !TESObjectCELL_IsInterior(ParentCell) ) /*0x5f967b*/
  {
    v7 = (int *)bhkWorldM; /*0x5f968d*/
LABEL_6:
    v96.member.super.flags = (TESForm::FormFlags)v7; /*0x5f9697*/
    goto LABEL_7; /*0x5f9697*/
  }
  v7 = (int *)sub_424180(v6 + 2); /*0x5f9685*/
  v96.member.super.flags = (TESForm::FormFlags)v7; /*0x5f9687*/
LABEL_7:
  vtbl = a1[1].vtbl; /*0x5f969b*/
  unk1F4 = (int *)TESDataHandler_g_PlayerRef->unk1F4; /*0x5f96a5*/
  LODWORD(v96.member.rot.y) = unk1F4; /*0x5f96ab*/
  if ( vtbl /*0x5f96c1*/
    && (v10 = (*((int (__thiscall **)(TESObjectREFRVtbl *, int))vtbl->super.super.InitializeComponent + 0x3B))(vtbl, 1)) != 0
    && *(_DWORD *)(v10 + 8) )
  {
    v11 = flt_B37D38; /*0x5f96c7*/
  }
  else
  {
    v11 = (double)dword_B37D30; /*0x5f96cf*/
  }
  *(float *)&v96.member.super.refID = v11; /*0x5f96d7*/
  if ( v7 && unk1F4 && !bhkSphereShapeProbeCollector_GetWorldFromPhantom(unk1F4) ) /*0x5f96eb*/
  {
    sub_5F11F0( /*0x5f9704*/
      (Actor *)a1,
      a3,
      (float *)&v96.member.baseExtraList.members.m_presenceBitfield[4],
      (float *)&v96.member.baseExtraList);
    bhkSphereShapeProbeCollector_GetPhantomTransform(unk1F4, v7); /*0x5f970c*/
    vtbl_high = HIWORD(MobileObject_GetCollisionFilterInfo(a1, &v96)->vtbl); /*0x5f971d*/
    if ( unk1F4[0x6A] != vtbl_high ) /*0x5f9727*/
      bhkSphereShapeProbeCollector_SetCollisionIdentityHigh16(unk1F4, vtbl_high); /*0x5f972c*/
    if ( !bhkSphereShapeProbeCollector_CastAlongVector( /*0x5f974c*/
            (float *)unk1F4,
            (float *)&v96.member.baseExtraList.members.m_presenceBitfield[4],
            (float *)&v96.member.baseExtraList,
            *(float *)&v96.member.super.refID) )
      goto LABEL_97; /*0x5f974c*/
    v13 = unk1F4; /*0x5f9752*/
    sub_4806E0(*(_DWORD *)(unk1F4[4] + 0x28)); /*0x5f975b*/
    if ( v14 ) /*0x5f9765*/
    {
      v15 = (Data *)sub_4DC270(v14); /*0x5f9770*/
      v96.member.super.modlist.data = v15; /*0x5f9772*/
    }
    else
    {
      v15 = 0; /*0x5f9778*/
      v96.member.super.modlist.data = 0; /*0x5f977a*/
    }
    v16 = v13[4]; /*0x5f977e*/
    v17 = *(_DWORD *)(v16 + 0x28); /*0x5f9781*/
    if ( *(_BYTE *)(v17 + 0x18) == 1 ) /*0x5f9788*/
    {
      v18 = v17 + *(_DWORD *)(v17 + 0x10); /*0x5f9791*/
      if ( v18 ) /*0x5f9793*/
      {
        v19 = a1->vtbl; /*0x5f979c*/
        v96.member.super.refID = *(_DWORD *)(v18 + 0xC); /*0x5f979e*/
        v94 = 1; /*0x5f97aa*/
        v20 = (int)v19->GetNiNode(a1); /*0x5f97af*/
        if ( v20 ) /*0x5f97b3*/
        {
          v21 = (NiTransform *)(v20 + 0x64); /*0x5f97b8*/
          v22 = (NiPoint3 *)(*((int (__thiscall **)(TESObjectREFRVtbl *))a1[1].vtbl->super.super.InitializeComponent /*0x5f97c3*/
                             + 0xC4))(a1[1].vtbl);
          v23 = sub_53D4B0(v21, (float *)&v96.member.childCell, v22); /*0x5f97cd*/
          v24 = *v23; /*0x5f97d2*/
          v25 = v23[1]; /*0x5f97d4*/
          v26 = v23[2]; /*0x5f97d7*/
          v96.member.rot.z = v24; /*0x5f97da*/
          v96.member.pos[0] = v25; /*0x5f97e8*/
          v96.member.pos[1] = v26; /*0x5f97ec*/
          v27 = (NiPoint3 *)sub_5E6A40((Actor *)a1, a2); /*0x5f97f0*/
          v28 = sub_53D4B0(v21, v97, v27); /*0x5f9800*/
          v29 = *(TESObjectCELL *(__thiscall **)(TESChildCELL *))v28; /*0x5f9805*/
          v30 = *((TESForm **)v28 + 1); /*0x5f9807*/
          v31 = v28[2]; /*0x5f980a*/
          v96.member.childCell.GetChildCell = v29; /*0x5f980d*/
          v96.member.baseForm = v30; /*0x5f9811*/
          v96.member.rot.x = v31; /*0x5f9815*/
        }
        v95 = *(float *)&v96.member.childCell.GetChildCell - v96.member.rot.z; /*0x5f9825*/
        *(float *)&v96.member.super.type = *(float *)&v96.member.baseForm - v96.member.pos[0]; /*0x5f9831*/
        *(float *)&v96.member.super.modlist.next = v96.member.rot.x - v96.member.pos[1]; /*0x5f983d*/
        v96.member.rot.z = v95; /*0x5f9845*/
        v96.member.pos[0] = *(float *)&v96.member.super.type; /*0x5f984d*/
        v96.member.pos[1] = *(float *)&v96.member.super.modlist.next; /*0x5f9855*/
        sub_43F350(&v96.member.rot.z); /*0x5f9859*/
        sub_43F3E0(&v96.member.scale, *(__m128 **)(LODWORD(v96.member.rot.y) + 0x10)); /*0x5f986d*/
        v32 = a1[1].vtbl; /*0x5f9872*/
        if ( v32 /*0x5f988a*/
          && (v33 = (*((int (__thiscall **)(TESObjectREFRVtbl *, int))v32->super.super.InitializeComponent + 0x3B))(
                      v32,
                      1)) != 0 )
        {
          v34 = *(_DWORD *)(v33 + 8); /*0x5f988c*/
        }
        else
        {
          v34 = 0; /*0x5f9891*/
        }
        if ( v34 ) /*0x5f9895*/
        {
          v35 = *(float *)(v34 + 0x7C); /*0x5f9897*/
        }
        else
        {
          sub_5E4330(a1, 4); /*0x5f98a0*/
          if ( v36 ) /*0x5f98a7*/
          {
            v35 = *(float *)(*(_DWORD *)(v36 + 8) + 0x58); /*0x5f98ac*/
          }
          else
          {
            v35 = 0.0; /*0x5f98b1*/
            v94 = 0; /*0x5f98b3*/
          }
        }
        *(float *)&v96.member.super.type = v35; /*0x5f98bc*/
        v37 = *(_DWORD *)(*(_DWORD *)(LODWORD(v96.member.rot.y) + 0x10) + 0x2C); /*0x5f98d1*/
        *(float *)&v96.member.super.type = *(float *)&v96.member.super.type + dbl_A30E48; /*0x5f98d9*/
        v95 = 0.0; /*0x5f98dd*/
        if ( (*(_BYTE *)sub_497340((_DWORD *)v96.member.super.refID, &v96) & 0x3F) == 0x11 ) /*0x5f98f2*/
        {
          *(float *)&v38 = COERCE_FLOAT(sub_440AC0(TES, &v96.member.scale)); /*0x5f98ff*/
        }
        else
        {
          v39 = (int *)sub_494F10((_DWORD *)v96.member.super.refID); /*0x5f990a*/
          if ( !v39 /*0x5f992d*/
            || (v95 = *((float *)v39 + 4), v37 == 0xFFFFFFFF)
            || (v40 = (*(int (__thiscall **)(int *))(*v39 + 0x88))(v39)) == 0 )
          {
LABEL_41:
            SafeFloatPointer = GameSetting_GetSafeFloatPointer((int *)&flt_B37138); /*0x5f9940*/
            *(float *)&v96.member.super.type = sub_4AC760(*(float *)&v96.member.super.type, *(float *)SafeFloatPointer); /*0x5f9962*/
            v96.member.super.modlist.next = *(TESForm::ModReferenceList **)GameSetting_GetSafeFloatPointer((int *)&flt_B37130); /*0x5f9972*/
            v42 = *(float **)(v18 + 0x50); /*0x5f9976*/
            v43 = *(float *)&v96.member.super.modlist.next; /*0x5f9979*/
            *(float *)&v96.member.super.modlist.next = *(float *)&v96.member.super.modlist.next * v96.member.rot.z; /*0x5f9983*/
            v96.member.pos[2] = v96.member.pos[0] * v43; /*0x5f998d*/
            *(float *)&v96.vtbl = v43 * v96.member.pos[1]; /*0x5f9995*/
            *(float *)&v96.member.super.modlist.next = *(float *)&v96.member.super.modlist.next /*0x5f99a7*/
                                                     * *(float *)&v96.member.super.type;
            v96.member.pos[2] = v96.member.pos[2] * *(float *)&v96.member.super.type; /*0x5f99b1*/
            *(float *)&v96.vtbl = *(float *)&v96.member.super.type * *(float *)&v96.vtbl; /*0x5f99b9*/
            v96.member.childCell.GetChildCell = (TESObjectCELL *(__thiscall *)(TESChildCELL *))v96.member.super.modlist.next; /*0x5f99c1*/
            *(float *)&v96.member.baseForm = v96.member.pos[2]; /*0x5f99c9*/
            v96.member.rot.x = *(float *)&v96.vtbl; /*0x5f99d1*/
            *(float *)&v96.member.super.type = sub_89DA90(v42); /*0x5f99df*/
            v44 = *(float *)GameSetting_GetSafeFloatPointer((int *)&flt_B37140); /*0x5f99ec*/
            if ( v44 > *(float *)&v96.member.super.type ) /*0x5f99f5*/
            {
              v45 = GameSetting_GetSafeFloatPointer((int *)&flt_B37140); /*0x5f99fc*/
              *(float *)&v96.vtbl = *(float *)&v96.member.super.type / *(float *)v45; /*0x5f9a0c*/
              NiPoint3::MutliplyByValue((NiPoint3 *)&v96.member.childCell, *(float *)&v96.vtbl); /*0x5f9a17*/
            }
            if ( (*sub_497340((_DWORD *)v96.member.super.refID, &v96) & 0x3F) == 8 ) /*0x5f9a32*/
            {
              v46 = GameSetting_GetSafeFloatPointer((int *)&flt_B37148); /*0x5f9a39*/
              NiPoint3::MutliplyByValue((NiPoint3 *)&v96.member.childCell, *(float *)v46); /*0x5f9a48*/
            }
            sub_4529E0(v99, (float *)&v96.member.childCell); /*0x5f9a5a*/
            (*(void (__thiscall **)(TESForm::FormFlags))(*(_DWORD *)v96.member.super.flags + 0x58))(v96.member.super.flags); /*0x5f9a6b*/
            v47 = *(_DWORD *)(LODWORD(v96.member.rot.y) + 0x10); /*0x5f9a71*/
            sub_8A6410(v18); /*0x5f9a76*/
            (*(void (__thiscall **)(_DWORD, float *, int))(**(_DWORD **)(v18 + 0x50) + 0x60))( /*0x5f9a8c*/
              *(_DWORD *)(v18 + 0x50),
              v99,
              v47);
            v48 = ((double (__thiscall *)(TESForm::FormFlags))*(_DWORD *)(*(_DWORD *)v96.member.super.flags + 0x58))(v96.member.super.flags); /*0x5f9a97*/
            sub_5F05F0( /*0x5f9ac3*/
              (int)a1,
              v44,
              v48,
              SLODWORD(v96.member.scale),
              (int)v96.member.niNode,
              (int)v96.member.parentCell,
              (int)v96.member.super.modlist.data,
              (_DWORD *)v96.member.super.refID,
              SLODWORD(v95));
            if ( !v94 ) /*0x5f9acd*/
              goto LABEL_85; /*0x5f9acd*/
            v96.member.childCell.GetChildCell = (TESObjectCELL *(__thiscall *)(TESChildCELL *))LODWORD(v96.member.rot.z); /*0x5f9adf*/
            v96.member.baseForm = (TESForm *)LODWORD(v96.member.pos[0]); /*0x5f9ae7*/
            v96.member.rot.x = v96.member.pos[1]; /*0x5f9aee*/
            v84 = sub_47D9E0((float *)&v96.member.childCell, (float *)&v96.member.baseExtraList); /*0x5f9b04*/
            v49 = sub_47DA10(v97, v84, (float *)&v96.member.baseExtraList); /*0x5f9b08*/
            sub_43F320((float *)&v96.member.childCell, v49); /*0x5f9b15*/
            sub_43F350((float *)&v96.member.childCell); /*0x5f9b1e*/
            if ( (*sub_497340((_DWORD *)v96.member.super.refID, &v96) & 0x3F) == 8 ) /*0x5f9b39*/
            {
              data = v96.member.super.modlist.data; /*0x5f9b3b*/
              if ( !v96.member.super.modlist.data /*0x5f9b4d*/
                || !(*(unsigned __int8 (__thiscall **)(Data *))(v96.member.super.modlist.data->errorState + 0x190))(v96.member.super.modlist.data) )
              {
                v96.member.super.flags = sBloodParticleDefault; /*0x5f9b62*/
                goto LABEL_53; /*0x5f9b66*/
              }
              v51 = sub_5E1BF0(data); /*0x5f9b55*/
            }
            else
            {
              v51 = sub_5361B0(SLODWORD(v95)); /*0x5f9b6d*/
            }
            v96.member.super.flags = v51; /*0x5f9b75*/
LABEL_53:
            if ( v96.member.super.flags ) /*0x5f9b7e*/
            {
              TESObjectREFR_GetParentCell(a1); /*0x5f9b86*/
              v85 = sub_4C9BE0(a1); /*0x5f9b96*/
              v52 = TESObjectREFR_GetParentCell(a1); /*0x5f9b99*/
              v53 = sub_441800(v52, v85, 3u); /*0x5f9ba7*/
              v54 = (TESObjectREFRVtbl *)FormHeapAlloc(0x20u); /*0x5f9bae*/
              v96.vtbl = v54; /*0x5f9bb3*/
              v55 = 0; /*0x5f9bb7*/
              v100 = 0; /*0x5f9bbb*/
              if ( v54 ) /*0x5f9bc2*/
              {
                scale = v96.member.scale; /*0x5f9bdf*/
                niNode = v96.member.niNode; /*0x5f9be5*/
                v82 = v96.member.parentCell; /*0x5f9bec*/
                GetChildCell = v96.member.childCell.GetChildCell; /*0x5f9bf8*/
                baseForm = v96.member.baseForm; /*0x5f9bfe*/
                x_low = LODWORD(v96.member.rot.x); /*0x5f9c01*/
                flags = (const char *)v96.member.super.flags; /*0x5f9c08*/
                v72 = flt_A31E2C; /*0x5f9c0d*/
                v56 = TESObjectREFR_GetParentCell(a1); /*0x5f9c10*/
                v55 = sub_5713F0( /*0x5f9c18*/
                        v54,
                        (int)v56,
                        v72,
                        v53,
                        flags,
                        *(float *)&GetChildCell,
                        *(float *)&baseForm,
                        x_low,
                        scale,
                        (UInt32)niNode,
                        (const char *)v82,
                        1.0,
                        1);
              }
              v100 = 0xFFFFFFFF; /*0x5f9c23*/
              sub_678D30((int *)&ActorProcessManager_ptr, (volatile LONG *)v55); /*0x5f9c2e*/
            }
            goto LABEL_85; /*0x5f9c33*/
          }
          *(float *)&v38 = COERCE_FLOAT((*(int (__thiscall **)(int, int))(*(_DWORD *)v40 + 0x9C))(v40, v37)); /*0x5f993a*/
        }
        v95 = *(float *)&v38; /*0x5f993c*/
        goto LABEL_41; /*0x5f993c*/
      }
    }
    if ( a1 == (TESObjectREFR *)TESDataHandler_g_PlayerRef ) /*0x5f9c3e*/
      goto LABEL_87; /*0x5f9c3e*/
    v57 = sub_47DDE0(*(_DWORD *)(v16 + 0x28)); /*0x5f9c45*/
    if ( v57 ) /*0x5f9c4f*/
      v58 = *(_DWORD **)(v57 + 0xC); /*0x5f9c51*/
    else
      v58 = 0; /*0x5f9c56*/
    if ( !v15 /*0x5f9ca6*/
      || !(*(unsigned __int8 (__thiscall **)(Data *))(v15->errorState + 0x190))(v15)
      || (*(unsigned __int8 (__thiscall **)(Data *, _DWORD))(v15->errorState + 0x198))(v15, 0)
      || !v58
      || (*(_BYTE *)sub_497340(v58, &v96) & 0x3F) != 0x14 )
    {
LABEL_86:
      if ( a1 != (TESObjectREFR *)TESDataHandler_g_PlayerRef ) /*0x5f9e0d*/
      {
LABEL_97:
        bhkSphereShapeProbeCollector_GetPhantomTransform((int *)LODWORD(v96.member.rot.y), 0); /*0x5f9e6f*/
        return HIBYTE(v93); /*0x5f9e75*/
      }
LABEL_87:
      if ( v15 && (v68 = TESObjectREFR_GetParentCell((TESObjectREFR *)v15)) != 0 && (sub_4440C0(v68), v69) ) /*0x5f9e27*/
        v70 = *(_DWORD **)(v69 + 0x24); /*0x5f9e29*/
      else
        v70 = 0; /*0x5f9e2e*/
      if ( v70 ) /*0x5f9e32*/
      {
        if ( v15 == (Data *)InterfaceManager_GetSingleton(0, 1)->unk0C0[2] ) /*0x5f9e46*/
        {
          if ( sub_536AE0(v70, (int)v15) ) /*0x5f9e4b*/
          {
            if ( v15 != (Data *)0xFFFFFFBC ) /*0x5f9e59*/
            {
              Script_AddEventToExtraScript(v15, &v15->name[0x28], 0x10000000);// RealArenaTraining: player static reach probe event. Args: source/ref=EDI, targetExtra=EDI+0x44, mask=0x10000000. Used for arena bag/doll melee props after player and crosshair/ref checks. /*0x5f9e62*/
              HIBYTE(v93) = 1; /*0x5f9e6a*/
            }
          }
        }
      }
      goto LABEL_97; /*0x5f9e6a*/
    }
    v59 = a1[1].vtbl; /*0x5f9cac*/
    if ( v59 ) /*0x5f9cb1*/
    {
      v60 = (*((int (__thiscall **)(TESObjectREFRVtbl *, int))v59->super.super.InitializeComponent + 0x3B))(v59, 1); /*0x5f9cbd*/
      if ( v60 ) /*0x5f9cc1*/
      {
        v61 = *(_DWORD *)(v60 + 8); /*0x5f9cc3*/
        goto LABEL_71; /*0x5f9cc6*/
      }
    }
    else
    {
      v60 = 0; /*0x5f9cc8*/
    }
    v61 = 0; /*0x5f9cca*/
LABEL_71:
    if ( v61 ) /*0x5f9cce*/
      v62 = *(char *)(v61 + 0x90); /*0x5f9cd0*/
    else
      v62 = 0xFFFFFFFF; /*0x5f9cd9*/
    v95 = 0.0; /*0x5f9ce0*/
    if ( v60 ) /*0x5f9ce4*/
    {
      EquippedWeaponData_GetDamage( /*0x5f9cef*/
        v60,
        (int)&savedregs,
        (int *)a1,
        COERCE_INT(1.0),
        v88,
        v89,
        v90,
        v91,
        v92,
        v93,
        v95,
        *(float *)&v96.vtbl);
      v95 = 1.0; /*0x5f9cf4*/
    }
    else if ( Actor_IsCreature((Actor *)a1) ) /*0x5f9cfc*/
    {
      v96.vtbl = (TESObjectREFRVtbl *)((int (__thiscall *)(TESObjectREFR *))a1->vtbl[1].IsParalyzed)(a1); /*0x5f9d11*/
      v95 = (float)(int)v96.vtbl; /*0x5f9d19*/
    }
    else
    {
      (*(void (__thiscall **)(Data *, float *, TESForm::FormFlags *))(v15->errorState + 0x19C))( /*0x5f9d33*/
        v15,
        &v95,
        &v96.member.super.flags);
      FatigueFraction = Actor_GetFatigueFraction((Actor *)a1, v62, (int)v15); /*0x5f9d46*/
      v83 = ((int (__thiscall *)(TESObjectREFR *))a1->vtbl[1].Unk_37)(a1); /*0x5f9d51*/
      v79 = ((int (__thiscall *)(TESObjectREFR *))a1->vtbl[1].Unk_37)(a1); /*0x5f9d60*/
      v63 = ((int (__thiscall *)(TESObjectREFR *))a1->vtbl[1].Unk_37)(a1); /*0x5f9d6b*/
      Calc_HandToHandDamage(v63, 0x11, v79, COERCE_FLOAT(7), v83, 0, (float *)LODWORD(FatigueFraction)); /*0x5f9d6e*/
    }
    if ( Actor_IsCreature((Actor *)v15) ) /*0x5f9d78*/
    {
      v86 = 0; /*0x5f9d83*/
      v80 = v62; /*0x5f9d89*/
      v77 = v15; /*0x5f9d8a*/
    }
    else
    {
      v64 = sub_5E5A00(v96.member.super.modlist.data); /*0x5f9d91*/
      v65 = (unsigned int *)v64; /*0x5f9d98*/
      v86 = 1; /*0x5f9d9c*/
      if ( v64 ) /*0x5f9da0*/
      {
        v66 = TESObjectARMO_ISHeavyArmor(v64[2]); /*0x5f9da5*/
        sub_6AF880( /*0x5f9dc3*/
          v11,
          st6_0,
          (int)a1,
          v95,
          SLODWORD(v95),
          (int)v96.member.super.modlist.data,
          v62,
          v66,
          0xFFFFFFFF,
          1,
          0);
        ContainerEntryExtraData_DestroyDataTable(v65, v67); /*0x5f9dcd*/
        FormHeapFree((unsigned int)v65); /*0x5f9dd3*/
LABEL_85:
        v15 = v96.member.super.modlist.data; /*0x5f9dfe*/
        HIBYTE(v93) = 1; /*0x5f9e02*/
        goto LABEL_86; /*0x5f9e02*/
      }
      v80 = v62; /*0x5f9de3*/
      v77 = v96.member.super.modlist.data; /*0x5f9de4*/
    }
    sub_6AF880(v11, st6_0, (int)a1, v95, COERCE_INT(0.0), (int)v77, v80, 0xFFFFFFFF, 0xFFFFFFFF, v86, 0); /*0x5f9df6*/
    goto LABEL_85; /*0x5f9df6*/
  }
  return HIBYTE(v93); /*0x5f9e7e*/
}
