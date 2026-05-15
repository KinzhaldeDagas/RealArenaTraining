// 0x005FEF00 @ 0x005FEF00
void __userpurge Actor_AttackHandling(
        Actor *a1@<ecx>,
        double st6_0@<st1>,
        double a3@<st0>,
        int a4,
        int a5,
        int a6,
        int a7,
        int a8,
        int a9,
        int a10,
        int a11,
        int a12,
        int a13,
        int a14,
        int a15,
        int a16,
        float a17,
        int a18,
        int a19,
        int a20,
        int a21,
        int a22,
        float a23)
{
  LowProcess *process; // ecx
  int v25; // eax
  int v26; // eax
  int v27; // eax
  bool v28; // cc
  int v29; // eax
  NiTransform *v30; // ebp
  NiPoint3 *v31; // eax
  float *v32; // eax
  float v33; // ecx
  float v34; // edx
  NiPoint3 *v35; // eax
  int *v36; // eax
  int v37; // ecx
  int v38; // edx
  double v39; // st6
  double v40; // rt2
  NiAVObject *v41; // eax
  int v42; // ecx
  int v43; // ebp
  int v44; // eax
  NiObjectNET *v45; // eax
  BSShaderProperty *v46; // esi
  BSShaderProperty *v47; // eax
  UInt16 v48; // dx
  NiObjectNET *v49; // eax
  BSShaderProperty *v50; // esi
  UInt16 v51; // ax
  float v52; // [esp+44h] [ebp-1C0h]
  float v53; // [esp+44h] [ebp-1C0h]
  float v54; // [esp+44h] [ebp-1C0h]
  int v55; // [esp+64h] [ebp-1A0h] BYREF
  int v56; // [esp+68h] [ebp-19Ch]
  int v57; // [esp+6Ch] [ebp-198h]
  char v58; // [esp+73h] [ebp-191h]
  float a2[3]; // [esp+74h] [ebp-190h] BYREF
  float v60; // [esp+80h] [ebp-184h]
  float v61; // [esp+8Ch] [ebp-178h] BYREF
  float v62; // [esp+90h] [ebp-174h]
  float v63; // [esp+94h] [ebp-170h]
  float v64; // [esp+9Ch] [ebp-168h] BYREF
  float v65; // [esp+A0h] [ebp-164h]
  float v66; // [esp+A4h] [ebp-160h]
  float v67; // [esp+A8h] [ebp-15Ch]
  float v68; // [esp+ACh] [ebp-158h] BYREF
  float v69; // [esp+B0h] [ebp-154h]
  float v70; // [esp+B4h] [ebp-150h]
  float v71; // [esp+B8h] [ebp-14Ch]
  int v72; // [esp+200h] [ebp-4h]

  if ( a6 && BaseExtraList_HasGhost((_BYTE *)(a6 + 0x44)) ) /*0x5fec46*/
  {
    process = a1->members.super.process; /*0x5fec4f*/
    if ( process && (v25 = (int)process->GetEquippedWeaponData(process, 1)) != 0 ) /*0x5fec64*/
      v26 = *(_DWORD *)(v25 + 8); /*0x5fec66*/
    else
      v26 = 0; /*0x5fec6b*/
    if ( v26 ) /*0x5fec6f*/
      v27 = *(char *)(v26 + 0x90); /*0x5fec71*/
    else
      v27 = 0xFFFFFFFF; /*0x5fec7a*/
    sub_6AF880(0.0, st6_0, (int)a1, 0.0, COERCE_INT(0.0), 0, v27, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0); /*0x5fec95*/
    Actor_AttackHandling_::Done(a4, a5, a6); /*0x5fec9d*/
  }
  else
  {
    v28 = dword_B148CC <= 1; /*0x5feca2*/
    v58 = 0; /*0x5fecad*/
    if ( v28 || a5 ) /*0x5fecba*/
    {
      Actor_AttackHandling_::EquippedWeaponAndAttackReach(a1); /*0x5fef86*/
    }
    else
    {
      if ( a1 == (Actor *)TESDataHandler_g_PlayerRef ) /*0x5fecc6*/
      {
        v64 = 0.0; /*0x5fecca*/
        v65 = 0.0; /*0x5fecd2*/
        v68 = 0.0; /*0x5fecda*/
        v66 = 1.0; /*0x5fece1*/
        v67 = 1.0; /*0x5fecef*/
        v69 = 0.0; /*0x5fecfd*/
        v70 = 1.0; /*0x5fed04*/
      }
      else
      {
        v64 = 1.0; /*0x5fed14*/
        v67 = 1.0; /*0x5fed1c*/
        v68 = 1.0; /*0x5fed23*/
        v65 = 0.0; /*0x5fed31*/
        v66 = 0.0; /*0x5fed39*/
        v69 = 0.0; /*0x5fed47*/
        v70 = 0.0; /*0x5fed4e*/
      }
      v71 = 1.0; /*0x5fed0b*/
      v29 = ((int (__usercall *)@<eax>(Actor *@<ecx>, double@<st0>, double@<st1>))a1->vtbl->super.super.GetNiNode)( /*0x5fed66*/
              a1,
              a3,
              st6_0);
      if ( v29 ) /*0x5fed6a*/
      {
        v30 = (NiTransform *)(v29 + 0x64); /*0x5fed71*/
        v31 = (NiPoint3 *)a1->members.super.process->GetUnk20C(a1->members.super.process); /*0x5fed7a*/
        v32 = sub_53D4B0(v30, (float *)&v55, v31); /*0x5fed84*/
        v33 = v32[1]; /*0x5fed8b*/
        v61 = *v32; /*0x5fed8e*/
        v34 = v32[2]; /*0x5fed92*/
        v62 = v33; /*0x5fed99*/
        v63 = v34; /*0x5feda0*/
        v35 = (NiPoint3 *)sub_5E6A40(a1, a2); /*0x5feda4*/
        v36 = (int *)sub_53D4B0(v30, &v64, v35); /*0x5fedb1*/
        v37 = v36[1]; /*0x5fedb8*/
        v55 = *v36; /*0x5fedbb*/
        v38 = v36[2]; /*0x5fedbf*/
        v56 = v37; /*0x5fedc2*/
        v57 = v38; /*0x5fedc6*/
      }
      a2[0] = *(float *)&v55 - v61; /*0x5fedd2*/
      v52 = *(float *)&v56 - v62; /*0x5fedde*/
      v60 = *(float *)&v57 - v63; /*0x5fedea*/
      v61 = a2[0]; /*0x5fedfe*/
      a2[1] = v52; /*0x5fee02*/
      v62 = v52; /*0x5fee0e*/
      a2[2] = v60; /*0x5fee12*/
      v63 = v60; /*0x5fee1e*/
      sub_43F350(&v61); /*0x5fee22*/
      v39 = dbl_A6E6F8; /*0x5fee34*/
      a2[0] = v61 * v39; /*0x5fee44*/
      v53 = v62 * v39; /*0x5fee4e*/
      v60 = v39 * v63; /*0x5fee56*/
      v40 = dbl_A2FC80; /*0x5fee66*/
      a2[0] = a2[0] * v40; /*0x5fee68*/
      v54 = v53 * v40; /*0x5fee72*/
      v60 = v40 * v60; /*0x5fee7a*/
      v61 = a2[0]; /*0x5fee82*/
      v62 = v54; /*0x5fee8a*/
      v63 = v60; /*0x5fee92*/
      v41 = sub_6FCDC0(&v61, (int *)&v68); /*0x5fee96*/
      v42 = v57; /*0x5fee9f*/
      v43 = (int)v41; /*0x5feea3*/
      v44 = v56; /*0x5feea5*/
      *(float *)(v43 + 0x54) = *(float *)&v55; /*0x5feea9*/
      *(_DWORD *)(v43 + 0x58) = v44; /*0x5feeac*/
      *(_DWORD *)(v43 + 0x5C) = v42; /*0x5feeb1*/
      *(float *)&v45 = COERCE_FLOAT(FormHeapAlloc(0x1Cu)); /*0x5feeb4*/
      v46 = (BSShaderProperty *)v45; /*0x5feeb9*/
      v55 = (int)v45; /*0x5feebe*/
      v72 = 0; /*0x5feec4*/
      if ( *(float *)&v45 == 0.0 ) /*0x5feecf*/
      {
        v47 = 0; /*0x5feee8*/
      }
      else
      {
        NiObjectNET::NiObjectNET(v45); /*0x5feed3*/
        v46->vtbl = &NiVertexColorProperty::`vftable'; /*0x5feed8*/
        v46->member.super.flags = 8; /*0x5feede*/
        v47 = v46; /*0x5feee4*/
      }
      v48 = v47->member.super.flags & 0xFFC7 | 0x10; /*0x5feef3*/
      v72 = 0xFFFFFFFF; /*0x5feefa*/
      v47->member.super.flags = v48; /*0x5fef05*/
      sub_405680((NiNode *)v43, v47); /*0x5fef09*/
      *(float *)&v49 = COERCE_FLOAT(FormHeapAlloc(0x1Cu)); /*0x5fef10*/
      v50 = (BSShaderProperty *)v49; /*0x5fef15*/
      v55 = (int)v49; /*0x5fef1a*/
      v72 = 1; /*0x5fef20*/
      if ( *(float *)&v49 == 0.0 ) /*0x5fef2b*/
      {
        v50 = 0; /*0x5fef42*/
      }
      else
      {
        NiObjectNET::NiObjectNET(v49); /*0x5fef2f*/
        v50->vtbl = &NiZBufferProperty::`vftable'; /*0x5fef34*/
        v50->member.super.flags = 0xF; /*0x5fef3a*/
      }
      v51 = v50->member.super.flags & 0xFFFC | 2; /*0x5fef4c*/
      v72 = 0xFFFFFFFF; /*0x5fef53*/
      v50->member.super.flags = v51; /*0x5fef5e*/
      sub_405680((NiNode *)v43, v50); /*0x5fef62*/
      sub_440E60(TES, v43, flt_B148D4); /*0x5fef78*/
      Actor_AttackHandling_::EquippedWeaponAndAttackReach(a1); /*0x5fef81*/
    }
  }
}
