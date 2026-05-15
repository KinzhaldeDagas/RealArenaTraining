// 0x0060A640 @ 0x0060A640
char __thiscall sub_60A640(
        Actor *this,
        float *arg0,
        void (__thiscall **a3)(MagicCaster *this, MagicCaster *this),
        TESObjectREFR *a4)
{
  MagicCasterVtbl *v5; // eax
  MagicCasterVtbl *vtbl; // eax
  float *v7; // eax
  float *p_IsMagicItemUsable; // eax
  bhkCharacterProxy *CharProxy; // eax
  bhkCharacterProxy *v10; // eax
  char *v11; // eax
  __m128 *v12; // eax
  __m128 v13; // xmm0
  double v14; // st6
  ActorVtbl *v15; // eax
  NiNode *(__thiscall *GetNiNode)(TESObjectREFR *); // edx
  int v17; // eax
  NiTransform *v18; // eax
  bool (__thiscall **v19)(MagicCaster *, MagicItem *, float *, UInt32 *, bool); // ecx
  PlayerCharacter *v20; // esi
  bool v21; // zf
  NiNode *v22; // eax
  UInt32 v23; // ecx
  UInt32 v24; // edi
  int v25; // eax
  TESForm *v26; // eax
  PlayerCharacter *v27; // ecx
  NiNode *niNode; // edi
  int *SafeFloatPointer; // eax
  float *(__thiscall *GetPos)(TESObjectREFR *); // eax
  double v31; // rt0
  float *v32; // eax
  double v33; // st7
  PlayerCharacter *v34; // ecx
  float (__thiscall *GetZRotation)(MobileObject *); // eax
  float *(__thiscall *v36)(TESObjectREFR *); // eax
  double v37; // rt2
  float *v38; // eax
  TESObjectREFR *v39; // esi
  int v40; // edx
  double v41; // st7
  unsigned __int8 (__thiscall *v42)(_DWORD, int); // eax
  int v43; // ecx
  int v44; // eax
  int v45; // eax
  float *v46; // esi
  float *v47; // eax
  void (__thiscall **p_AddDisease)(MagicCaster *, MagicItemForm *, MagicTarget *, bool); // ecx
  float *v49; // esi
  int v50; // eax
  int v51; // eax
  LowProcess *v52; // ecx
  float v53; // ecx
  float *v54; // edi
  float v55; // eax
  float v56; // edx
  double v57; // st7
  char v58; // al
  double v59; // rt0
  int v60; // eax
  int v61; // ecx
  Atmosphere *v62; // eax
  NiAVObject *v63; // esi
  double v64; // rt1
  float *v65; // eax
  void (__thiscall **v66)(MagicCaster *, MagicItemForm *, MagicTarget *, bool); // ecx
  float *v67; // eax
  float (__thiscall *v68)(MagicCaster *, bool, float); // edx
  NiTransform *v69; // eax
  MagicCasterVtbl *v70; // edx
  int v71; // eax
  int v72; // eax
  int v73; // eax
  int *v74; // eax
  int v75; // eax
  int v76; // eax
  int RandomLargeInteger; // eax
  int v78; // et2
  int (__thiscall **p_AddItem)(TESObjectREFR *, int); // edi
  int v80; // eax
  unsigned __int64 v82; // [esp-4h] [ebp-1D0h]
  float v83; // [esp+4h] [ebp-1C8h]
  __int64 v84; // [esp+8h] [ebp-1C4h]
  float v85; // [esp+10h] [ebp-1BCh]
  int a2; // [esp+14h] [ebp-1B8h]
  char *a2a; // [esp+14h] [ebp-1B8h]
  float v88; // [esp+18h] [ebp-1B4h]
  float v89; // [esp+18h] [ebp-1B4h]
  int v90; // [esp+18h] [ebp-1B4h]
  int *v91; // [esp+18h] [ebp-1B4h]
  bool v92; // [esp+36h] [ebp-196h]
  char v93; // [esp+36h] [ebp-196h]
  NiTransform v94; // [esp+38h] [ebp-194h] BYREF
  int v95; // [esp+6Ch] [ebp-160h] BYREF
  float v96[9]; // [esp+70h] [ebp-15Ch] BYREF
  float v97[9]; // [esp+94h] [ebp-138h] BYREF
  float v98[9]; // [esp+B8h] [ebp-114h] BYREF
  float v99[9]; // [esp+DCh] [ebp-F0h] BYREF
  NiTransform v100; // [esp+100h] [ebp-CCh] BYREF
  NiTransform v101; // [esp+134h] [ebp-98h] BYREF
  float v102[9]; // [esp+168h] [ebp-64h] BYREF
  __m128 v103; // [esp+18Ch] [ebp-40h] BYREF
  float v104; // [esp+19Ch] [ebp-30h] BYREF
  float v105; // [esp+1A0h] [ebp-2Ch]
  float v106; // [esp+1A4h] [ebp-28h]
  __m128 v107; // [esp+1ACh] [ebp-20h] BYREF

  LODWORD(v94.rot.data[2][2]) = a4; /*0x60a664*/
  v5 = (MagicCasterVtbl *)FormHeapAlloc(0x54u); /*0x60a668*/
  this->members.magicCaster.vtbl = v5; /*0x60a66d*/
  v5->AddAbility = 0; /*0x60a672*/
  this->members.magicCaster.vtbl->GetSpellEffectiveness = 0; /*0x60a677*/
  this->members.magicCaster.vtbl->AddEffectToSelf = 0; /*0x60a67d*/
  vtbl = this->members.magicCaster.vtbl; /*0x60a680*/
  vtbl->PlayTargettedCastAnim = *a3; /*0x60a688*/
  vtbl->PlayCastingAnim = a3[1]; /*0x60a68e*/
  vtbl->ApplyMagicItemCost = (void (__thiscall *)(MagicCaster *, MagicItem *, bool))a3[2]; /*0x60a694*/
  v7 = (float *)this->members.magicCaster.vtbl; /*0x60a69f*/
  v7[1] = *arg0; /*0x60a6a2*/
  v7[2] = arg0[1]; /*0x60a6a8*/
  v7[3] = arg0[2]; /*0x60a6b1*/
  qmemcpy(&this->members.magicCaster.vtbl->GetActiveMagicItem, &stru_B26AF0[0xA].unk2C, 0x24u); /*0x60a6c4*/
  p_IsMagicItemUsable = (float *)&this->members.magicCaster.vtbl->IsMagicItemUsable; /*0x60a6cf*/
  *p_IsMagicItemUsable = Vector3_InitValue_; /*0x60a6d2*/
  p_IsMagicItemUsable[1] = *(&Vector3_InitValue_ + 1); /*0x60a6da*/
  p_IsMagicItemUsable[2] = dword_B3F9B0; /*0x60a6e8*/
  CharProxy = MobileObject_GetCharProxy((MobileObject *)this); /*0x60a6eb*/
  if ( CharProxy ) /*0x60a6f2*/
  {
    v10 = MobileObject_GetCharProxy((MobileObject *)this); /*0x60a6fa*/
    if ( v10 && (v11 = *((char **)v10 + 2)) != 0 ) /*0x60a708*/
      v12 = (__m128 *)sub_8AC0A0(v11); /*0x60a70c*/
    else
      v12 = (__m128 *)&stru_BA7A40; /*0x60a713*/
    v13 = *v12; /*0x60a718*/
    v104 = v12->m128_f32[0]; /*0x60a71b*/
    v14 = flt_A7DEB4; /*0x60a72b*/
    v103 = v13; /*0x60a731*/
    if ( -v14 == v104 ) /*0x60a742*/
    {
      CharProxy = (bhkCharacterProxy *)this->vtbl->super.super.GetNiNode(this); /*0x60a752*/
      if ( CharProxy ) /*0x60a756*/
      {
        v15 = this->vtbl; /*0x60a75f*/
        v94.rot.data[1][0] = *(float *)&this->members.magicTarget.unk04; /*0x60a761*/
        GetNiNode = v15->super.super.GetNiNode; /*0x60a765*/
        v103.m128_f32[0] = v94.rot.data[1][0] * *(float *)&dword_B258DC; /*0x60a779*/
        v103.m128_f32[1] = v94.rot.data[1][0] * *(float *)&qword_B258E0; /*0x60a788*/
        v103.m128_f32[2] = v94.rot.data[1][0] * *((float *)&qword_B258E0 + 1); /*0x60a795*/
        v17 = (int)GetNiNode((TESObjectREFR *)this); /*0x60a79c*/
        v18 = sub_7101F0((NiTransform *)(v17 + 0x64), (NiTransform *)&v104, (NiPoint3 *)&v103); /*0x60a7b1*/
        v19 = &this->members.magicCaster.vtbl->IsMagicItemUsable; /*0x60a7bb*/
        *v19 = (bool (__thiscall *)(MagicCaster *, MagicItem *, float *, UInt32 *, bool))LODWORD(v18->rot.data[0][0]); /*0x60a7be*/
        v19[1] = (bool (__thiscall *)(MagicCaster *, MagicItem *, float *, UInt32 *, bool))LODWORD(v18->rot.data[0][1]); /*0x60a7c3*/
        CharProxy = (bhkCharacterProxy *)LODWORD(v18->rot.data[0][2]); /*0x60a7c6*/
        v19[2] = (bool (__thiscall *)(MagicCaster *, MagicItem *, float *, UInt32 *, bool))CharProxy; /*0x60a7c9*/
      }
    }
    else
    {
      LOBYTE(CharProxy) = (unsigned __int8)sub_43F3E0( /*0x60a7dd*/
                                             (float *)&this->members.magicCaster.vtbl->IsMagicItemUsable,
                                             &v103);
    }
  }
  v20 = (PlayerCharacter *)LODWORD(v94.rot.data[2][2]); /*0x60a7e5*/
  v21 = LODWORD(v94.rot.data[2][2]) == 0; /*0x60a7e9*/
  this->members.magicCaster.magicNode = (NiNode *)1; /*0x60a7eb*/
  if ( !v21 )
  {
    v22 = this->vtbl->super.super.GetNiNode(this); /*0x60a802*/
    v23 = this->members.unk070[2]; /*0x60a804*/
    LODWORD(v94.pos.x) = v22; /*0x60a809*/
    if ( v23 ) /*0x60a80d*/
    {
      if ( (*(int (__thiscall **)(UInt32))(*(_DWORD *)v23 + 0x330))(v23) ) /*0x60a817*/
      {
        v24 = this->members.unk070[2]; /*0x60a825*/
        a2 = (int)v20->vtbl->super.super.super.GetPos((TESObjectREFR *)v20); /*0x60a82e*/
        v25 = (*(int (__thiscall **)(UInt32))(*(_DWORD *)v24 + 0x330))(v24); /*0x60a839*/
        sub_618120(v25, v24, a2, COERCE_FLOAT(1)); /*0x60a83d*/
      }
    }
    this->members.magicCaster.vtbl->AddEffectToSelf = (void (__thiscall *)(MagicCaster *, ActiveEffect *))v20; /*0x60a845*/
    a2a = (char *)this->members.magicCaster.vtbl->AddEffectToSelf + 0x44; /*0x60a858*/
    v26 = this->vtbl->super.super.GetBaseForm(this); /*0x60a861*/
    Script_AddEventToExtraScript(v26, a2a, 0x100); /*0x60a864*/
    v27 = (PlayerCharacter *)this->members.unk070[2]; /*0x60a869*/
    if ( v27 ) /*0x60a871*/
    {
      if ( v27 == TESDataHandler_g_PlayerRef || v27->vtbl->super.IsInCombat((Actor *)v27, 1) ) /*0x60a889*/
      {
        v92 = v20->vtbl->super.super.super.IsDead((TESObjectREFR *)v20, 0); /*0x60a8ba*/
        (*(void (__thiscall **)(UInt32, _DWORD, Actor *, PlayerCharacter *))(*(_DWORD *)this->members.unk070[2] + 0x3AC))( /*0x60a8c6*/
          this->members.unk070[2],
          0,
          this,
          v20);
        if ( !v92 /*0x60a8fe*/
          && v20->vtbl->super.super.super.IsDead((TESObjectREFR *)v20, 0)
          && (!Actor_IsCreature((Actor *)v20)
           || ((unsigned __int8 (__thiscall *)(PlayerCharacter *))v20->vtbl->super.Unk_9E)(v20)) )
        {
          niNode = (NiNode *)v20->super.super.super.super.niNode; /*0x60a908*/
          if ( niNode ) /*0x60a90d*/
          {
            SafeFloatPointer = GameSetting_GetSafeFloatPointer((int *)&flt_B371F8); /*0x60a918*/
            v94.rot.data[1][0] = sub_547770(*(float *)SafeFloatPointer); /*0x60a929*/
            sub_8B8700(niNode); /*0x60a92d*/
            sub_88D070(niNode, 1, 1, 0); /*0x60a939*/
            NiAVObject_UpdateNiAVObject((NiAVObject *)niNode, 0.0, 0); /*0x60a94b*/
            sub_7101F0((NiTransform *)(LODWORD(v94.pos.x) + 0x64), &v94, (NiPoint3 *)&dword_B258DC); /*0x60a961*/
            sub_43F350((float *)&v94); /*0x60a96a*/
            GetPos = this->vtbl->super.super.GetPos; /*0x60a97d*/
            v31 = dbl_A3F410; /*0x60a987*/
            v107.m128_f32[0] = v94.rot.data[0][0] * v31; /*0x60a989*/
            v107.m128_f32[1] = v94.rot.data[0][1] * v31; /*0x60a996*/
            v107.m128_f32[2] = v31 * v94.rot.data[0][2]; /*0x60a9a1*/
            v32 = GetPos((TESObjectREFR *)this); /*0x60a9a8*/
            v103.m128_f32[0] = *v32 + v107.m128_f32[0]; /*0x60a9b3*/
            v103.m128_f32[1] = v32[1] + v107.m128_f32[1]; /*0x60a9cb*/
            v33 = v32[2]; /*0x60a9d9*/
            *(_QWORD *)&v94.rot.data[0][0] = v103.m128_u64[0]; /*0x60a9dc*/
            v103.m128_f32[2] = v33 + v107.m128_f32[2]; /*0x60a9f7*/
            LODWORD(v94.rot.data[0][2]) = v103.m128_i32[2]; /*0x60aa06*/
            sub_4529E0(&v104, (float *)&v94); /*0x60aa0a*/
            sub_5364B0((int)niNode, (__m128 *)&v104, v94.rot.data[1][0]); /*0x60aa22*/
          }
        }
      }
      else
      {
        (*(void (__thiscall **)(UInt32, _DWORD, Actor *, PlayerCharacter *))(*(_DWORD *)this->members.unk070[2] + 0x3B0))( /*0x60a89e*/
          this->members.unk070[2],
          0,
          this,
          v20);
      }
    }
    v34 = TESDataHandler_g_PlayerRef; /*0x60aa2a*/
    v21 = v20 == TESDataHandler_g_PlayerRef; /*0x60aa30*/
    v94.pos.y = 0.0; /*0x60aa32*/
    if ( v21 ) /*0x60aa3a*/
      LODWORD(v94.scale) = PlayerCharacter_GetPlayerNode(v34, 0); /*0x60aa43*/
    else
      LODWORD(v94.scale) = v20->super.super.super.super.niNode; /*0x60aa4c*/
    GetZRotation = this->vtbl->super.GetZRotation; /*0x60aa58*/
    *(float *)&v95 = flt_A427E0; /*0x60aa5e*/
    v88 = GetZRotation((MobileObject *)this); /*0x60aa6b*/
    NiMatrix33_InitRotationTransform(v96, v88); /*0x60aa6e*/
    v89 = sub_4A9720(this); /*0x60aa82*/
    NiMatrix33_InitRotationTransposedTransform___(v99, v89); /*0x60aa85*/
    qmemcpy(v96, NiMAtrix33_Multiply(v96, v97, v99), sizeof(v96)); /*0x60aaab*/
    v94.rot.data[0][0] = v96[1]; /*0x60aab1*/
    v94.rot.data[0][1] = v96[4]; /*0x60aab9*/
    v94.rot.data[0][2] = v96[7]; /*0x60aac1*/
    sub_4BFAA0((float *)&v94); /*0x60aaca*/
    v36 = this->vtbl->super.super.GetPos; /*0x60aad5*/
    v103.m128_f32[0] = v94.rot.data[0][0] * *(float *)&v95; /*0x60aaea*/
    v103.m128_f32[1] = v94.rot.data[0][1] * *(float *)&v95; /*0x60aaf7*/
    v103.m128_f32[2] = *(float *)&v95 * v94.rot.data[0][2]; /*0x60ab02*/
    v37 = dbl_A2FAA0; /*0x60ab18*/
    v94.rot.data[1][0] = v103.m128_f32[0] * v37; /*0x60ab1a*/
    v94.rot.data[1][1] = v103.m128_f32[1] * v37; /*0x60ab27*/
    v94.rot.data[1][2] = v37 * v103.m128_f32[2]; /*0x60ab32*/
    v38 = v36((TESObjectREFR *)this); /*0x60ab36*/
    v39 = (TESObjectREFR *)LODWORD(v94.rot.data[2][2]); /*0x60ab3a*/
    v40 = *(_DWORD *)LODWORD(v94.rot.data[2][2]); /*0x60ab42*/
    v107.m128_f32[0] = *v38 - v94.rot.data[1][0]; /*0x60ab48*/
    v93 = 0; /*0x60ab4f*/
    v107.m128_f32[1] = v38[1] - v94.rot.data[1][1]; /*0x60ab5b*/
    v41 = v38[2]; /*0x60ab62*/
    v42 = *(unsigned __int8 (__thiscall **)(_DWORD, int))(v40 + 0x198); /*0x60ab65*/
    v107.m128_f32[2] = v41 - v94.rot.data[1][2]; /*0x60ab6f*/
    LOBYTE(v43) = v42(LODWORD(v94.rot.data[2][2]), 1) == 0; /*0x60ab7a*/
    v90 = v43; /*0x60ab81*/
    v84 = *(_QWORD *)&v94.rot.data[0][0]; /*0x60ab90*/
    v85 = v94.rot.data[0][2]; /*0x60aba0*/
    v82 = v107.m128_u64[0]; /*0x60abaf*/
    v83 = v107.m128_f32[2]; /*0x60abbb*/
    v44 = (int)v39->vtbl->GetNiNode(v39); /*0x60abc8*/
    v45 = sub_609DF0( /*0x60abcd*/
            this,
            v44,
            *(float *)&v82,
            *((float *)&v82 + 1),
            v83,
            *(float *)&v84,
            *((float *)&v84 + 1),
            v85,
            (int)&v95,
            v90);
    LODWORD(v94.rot.data[1][0]) = v45; /*0x60abd4*/
    if ( v45 ) /*0x60abd8*/
    {
      v46 = (float *)(v45 + 0x64); /*0x60abe2*/
      v94.pos.z = *(float *)&v95 + dbl_A492D8; /*0x60abf5*/
      v103.m128_f32[0] = v94.rot.data[0][0] * v94.pos.z; /*0x60ac07*/
      v103.m128_f32[1] = v94.rot.data[0][1] * v94.pos.z; /*0x60ac14*/
      v103.m128_f32[2] = v94.pos.z * v94.rot.data[0][2]; /*0x60ac1f*/
      sub_718A80((float *)(v45 + 0x64), (float *)&v100); /*0x60ac26*/
      v104 = v103.m128_f32[0] + v107.m128_f32[0]; /*0x60ac49*/
      v105 = v103.m128_f32[1] + v107.m128_f32[1]; /*0x60ac65*/
      v106 = v103.m128_f32[2] + v107.m128_f32[2]; /*0x60ac7a*/
      v47 = sub_53D4B0(&v100, v103.m128_f32, (NiPoint3 *)&v104); /*0x60ac81*/
      p_AddDisease = &this->members.magicCaster.vtbl->AddDisease; /*0x60ac8b*/
      *p_AddDisease = *(void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v47; /*0x60ac8e*/
      p_AddDisease[1] = *((void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v47 + 1); /*0x60ac93*/
      p_AddDisease[2] = *((void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v47 + 2); /*0x60ac99*/
      sub_7102B0(v46, v98); /*0x60aca6*/
      v49 = NiMAtrix33_Multiply(v98, v97, (float *)(LODWORD(v94.pos.x) + 0x64)); /*0x60acc7*/
      v50 = LODWORD(v94.rot.data[1][0]); /*0x60acc9*/
      qmemcpy(&this->members.magicCaster.vtbl->GetActiveMagicItem, v49, 0x24u); /*0x60acd5*/
      this->members.magicCaster.vtbl->GetSpellEffectiveness = (float (__thiscall *)(MagicCaster *, bool, float))v50; /*0x60acdb*/
      v51 = sub_480340(v50); /*0x60acde*/
      if ( v51 ) /*0x60ace8*/
        v94.pos.y = *(float *)(v51 + 0x10); /*0x60aced*/
      v39 = (TESObjectREFR *)LODWORD(v94.rot.data[2][2]); /*0x60acf1*/
      v93 = 1; /*0x60acf5*/
    }
    if ( Actor_IsBlocking(v39) /*0x60ad25*/
      && (v52 = (LowProcess *)v39[1].vtbl) != 0
      && v52->GetEquippedShieldData(v52, 1)
      && sub_6131D0((int)v39, (int)this, 0) )
    {
      v53 = v39->member.pos[1]; /*0x60ad31*/
      v54 = arg0; /*0x60ad34*/
      v55 = v39->member.pos[0]; /*0x60ad37*/
      v56 = v39->member.pos[2]; /*0x60ad3d*/
      *(double *)&v94.rot.data[1][0] = arg0[2]; /*0x60ad40*/
      v105 = v53; /*0x60ad44*/
      v104 = v55; /*0x60ad4d*/
      v106 = v56; /*0x60ad54*/
      v57 = sub_5E0660(v39); /*0x60ad5b*/
      v94.pos.z = v57 * dbl_A2FAA0 + v106; /*0x60ad6d*/
      if ( v94.pos.z <= *(double *)&v94.rot.data[1][0] ) /*0x60ad7e*/
      {
        v58 = 1; /*0x60ad80*/
        goto LABEL_39; /*0x60ad82*/
      }
    }
    else
    {
      v54 = arg0; /*0x60ad84*/
    }
    v58 = 0; /*0x60ad87*/
LABEL_39:
    if ( v93 && !v58 ) /*0x60ad94*/
      goto LABEL_49; /*0x60ad94*/
    v59 = hkFactor; /*0x60ada6*/
    v104 = *v54 * v59; /*0x60ada8*/
    v105 = v54[1] * v59; /*0x60adb4*/
    v106 = v59 * v54[2]; /*0x60adbe*/
    v107.m128_f32[0] = v104; /*0x60adcc*/
    v107.m128_f32[1] = v105; /*0x60adda*/
    v107.m128_f32[2] = v106; /*0x60ade8*/
    v107.m128_f32[3] = flt_A57EF8; /*0x60adf5*/
    if ( v58 ) /*0x60adfc*/
    {
      v60 = sub_8AFB50(SLODWORD(v94.scale), 7); /*0x60ae05*/
    }
    else
    {
      LOBYTE(v61) = !v39->vtbl->IsDead(v39, 1); /*0x60ae23*/
      v60 = sub_8AFD70((float *)LODWORD(v94.scale), &v107, v61); /*0x60ae30*/
    }
    if ( v60 ) /*0x60ae3a*/
    {
      v91 = *(int **)(v60 + 8); /*0x60ae43*/
      LODWORD(v94.pos.y) = v60; /*0x60ae44*/
      v62 = (Atmosphere *)sub_47FA60(v91); /*0x60ae48*/
      if ( v62 ) /*0x60ae52*/
      {
        v63 = sub_452A60(v62); /*0x60ae69*/
        v64 = dbl_A492D8; /*0x60ae74*/
        v103.m128_f32[0] = v94.rot.data[0][0] * v64; /*0x60ae7a*/
        LODWORD(v94.rot.data[1][0]) = v63; /*0x60ae87*/
        v103.m128_f32[1] = v94.rot.data[0][1] * v64; /*0x60ae8d*/
        v103.m128_f32[2] = v64 * v94.rot.data[0][2]; /*0x60ae98*/
        sub_718A80((float *)&v63->members.m_worldTransform, (float *)&v101); /*0x60ae9f*/
        v104 = v63->members.m_worldTransform.pos.x + v103.m128_f32[0]; /*0x60aec1*/
        v105 = v63->members.m_worldTransform.pos.y + v103.m128_f32[1]; /*0x60aedc*/
        v106 = v63->members.m_worldTransform.pos.z + v103.m128_f32[2]; /*0x60aef0*/
        v65 = sub_53D4B0(&v101, v103.m128_f32, (NiPoint3 *)&v104); /*0x60aef7*/
        v66 = &this->members.magicCaster.vtbl->AddDisease; /*0x60af01*/
        *v66 = *(void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v65; /*0x60af04*/
        v66[1] = *((void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v65 + 1); /*0x60af09*/
        v66[2] = *((void (__thiscall **)(MagicCaster *, MagicItemForm *, MagicTarget *, bool))v65 + 2); /*0x60af0f*/
        sub_7102B0((float *)&v63->members.m_worldTransform, v97); /*0x60af19*/
        v67 = NiMAtrix33_Multiply(v97, v102, (float *)(LODWORD(v94.pos.x) + 0x64)); /*0x60af35*/
        v68 = (float (__thiscall *)(MagicCaster *, bool, float))LODWORD(v94.rot.data[1][0]); /*0x60af3d*/
        qmemcpy(&this->members.magicCaster.vtbl->GetActiveMagicItem, v67, 0x24u); /*0x60af4b*/
        v39 = (TESObjectREFR *)LODWORD(v94.rot.data[2][2]); /*0x60af50*/
        v54 = arg0; /*0x60af54*/
        this->members.magicCaster.vtbl->GetSpellEffectiveness = v68; /*0x60af57*/
      }
    }
    if ( this->members.magicCaster.vtbl->GetSpellEffectiveness )
    {
LABEL_49:
      if ( LODWORD(v94.pos.y) ) /*0x60af87*/
      {
        v69 = sub_7101F0((NiTransform *)(LODWORD(v94.pos.x) + 0x64), (NiTransform *)&v104, (NiPoint3 *)&dword_B258DC); /*0x60af9d*/
        sub_6088F0( /*0x60afd3*/
          (float *)this,
          *v54,
          v54[1],
          v54[2],
          v69->rot.data[0][0],
          v69->rot.data[0][1],
          v69->rot.data[0][2],
          (_DWORD *)LODWORD(v94.pos.y));
      }
      v70 = this->members.magicCaster.vtbl; /*0x60afd8*/
      if ( v70->GetSpellEffectiveness
        && (v71 = sub_47FAC0((int)v70->GetSpellEffectiveness)) != 0
        && (v72 = *(_DWORD *)(v71 + 0x10)) != 0
        && ((v73 = *(_DWORD *)(v72 + 8)) == 0 || (v74 = (int *)(v73 + 0x14)) == 0 || (v75 = *v74) == 0
          ? (v76 = 0)
          : (v76 = *(_DWORD *)(v75 + 8)),
            v76) )
      {
        switch ( *(_DWORD *)(v76 + 0x10) ) /*0x60b022*/
        {
          case 0: /*0x60b022*/
          case 3: /*0x60b022*/
          case 5: /*0x60b022*/
          case 0xA: /*0x60b022*/
          case 0xB: /*0x60b022*/
          case 0xD: /*0x60b022*/
          case 0xF: /*0x60b022*/
          case 0x12: /*0x60b022*/
          case 0x14: /*0x60b022*/
          case 0x19: /*0x60b022*/
          case 0x1A: /*0x60b022*/
          case 0x1C: /*0x60b022*/
          case 0x1E: /*0x60b022*/
            LOBYTE(CharProxy) = (unsigned __int8)sub_608DA0((MobileObject *)this, (int)v54, (int)a3); /*0x60b030*/
            break; /*0x60b049*/
          default:
            goto LABEL_62;
        }
      }
      else
      {
LABEL_62:
        LOBYTE(CharProxy) = Actor_IsCreature((Actor *)v39); /*0x60b04c*/
        if ( !(_BYTE)CharProxy || (LOBYTE(CharProxy) = v39->vtbl[1].super.GetName((TESForm *)v39), (_BYTE)CharProxy) ) /*0x60b065*/
        {
          if ( !this->members.unk07C ) /*0x60b08d*/
          {
            RandomLargeInteger = Game_RandomLargeInteger(0); /*0x60b095*/
            v78 = RandomLargeInteger % 0x64; /*0x60b0a0*/
            CharProxy = (bhkCharacterProxy *)(RandomLargeInteger / 0x64); /*0x60b0a0*/
            if ( v78 < dword_B370C8 ) /*0x60b0ab*/
            {
              p_AddItem = (int (__thiscall **)(TESObjectREFR *, int))&v39->vtbl->AddItem; /*0x60b0bd*/
              v80 = ((int (__thiscall *)(Actor *, _DWORD, int))this->vtbl->super.super.GetBaseForm)(this, 0, 1); /*0x60b0c3*/
              LOBYTE(CharProxy) = (*p_AddItem)(v39, v80); /*0x60b0ca*/
              BYTE1(this->members.avModifiers.fatigue) = 1; /*0x60b0cc*/
            }
          }
        }
        else
        {
          LOBYTE(CharProxy) = (unsigned __int8)sub_6079A0((MobileObject *)this, (int)v39); /*0x60b06a*/
          this->members.magicCaster.magicNode = (NiNode *)3; /*0x60b06f*/
        }
      }
    }
    else
    {
      LOBYTE(CharProxy) = (unsigned __int8)sub_6079A0((MobileObject *)this, (int)v39); /*0x60af66*/
    }
  }
  return (char)CharProxy; /*0x60af6b*/
}
