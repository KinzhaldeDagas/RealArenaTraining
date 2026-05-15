// 0x0060B120 @ 0x0060B120
void __thiscall sub_60B120(ArrowProjectile *this, int *arg0, float *arg4, TESChildCELL *arg8, int a5)
{
  ArrowProjectile_CollisionData *v6; // eax
  ArrowProjectile_CollisionData *unk05C; // eax
  ArrowProjectile_CollisionData *v8; // eax
  float *v9; // eax
  bhkCharacterProxy *CharProxy; // eax
  char *v11; // eax
  float *v12; // eax
  __int128 v13; // xmm0
  double v14; // st6
  NiNode *(__thiscall *GetNiNode)(TESObjectREFR *); // edx
  double speed; // st7
  int v17; // eax
  NiTransform *v18; // eax
  float *v19; // ecx
  NiNode *(__thiscall *v20)(TESObjectREFR *); // edx
  int v21; // eax
  TESChildCELL *v22; // esi
  Actor *shooter; // ecx
  Actor *v24; // edi
  int v25; // eax
  TESForm *v26; // eax
  Atmosphere *v27; // eax
  NiAVObject *v28; // eax
  ArrowProjectile_CollisionData *v29; // eax
  int v30; // eax
  int v31; // eax
  int *v32; // eax
  int v33; // eax
  float *v34; // esi
  int v35; // eax
  __m128 *v36; // eax
  float v37; // edi
  int v38; // eax
  ArrowProjectile_CollisionData *v39; // eax
  double v40; // st7
  double v41; // st5
  double v42; // st6
  double v43; // st7
  const char *v44; // edi
  TESObjectCELL *ParentCell; // eax
  TESObjectCELL *v46; // eax
  float *v47; // eax
  Actor *v48; // ecx
  int v49; // eax
  MagicCaster *p_magicCaster; // ecx
  EnchantmentItem *arrowEnch; // eax
  void (__thiscall **p_SetCastingTarget)(MagicCaster *, MagicTarget *); // edi
  int v53; // eax
  TESForm *v54; // eax
  AlchemyItem *poison; // edx
  void (__thiscall **v56)(MagicCaster *, MagicTarget *); // edi
  int v57; // eax
  TESForm *v58; // eax
  EnchantmentItem *bowEnch; // edx
  void (__thiscall **v60)(MagicCaster *, MagicTarget *); // edi
  int v61; // eax
  TESForm *v62; // eax
  TESChildCELL *v63; // edi
  TESObjectCELL *v64; // eax
  int v65; // eax
  _DWORD *v66; // ecx
  NiTransform *p_m_worldTransform; // edi
  NiTransform *v68; // eax
  int v69; // eax
  float x; // esi
  float y; // eax
  float z; // edx
  float v73; // [esp+0h] [ebp-260h]
  int v74; // [esp+4h] [ebp-25Ch]
  int v75; // [esp+18h] [ebp-248h]
  UInt32 v76; // [esp+1Ch] [ebp-244h]
  const char *v77; // [esp+20h] [ebp-240h]
  int v78; // [esp+24h] [ebp-23Ch]
  float *a2a; // [esp+24h] [ebp-23Ch]
  signed int a2b; // [esp+24h] [ebp-23Ch]
  float v81; // [esp+48h] [ebp-218h]
  float v82; // [esp+48h] [ebp-218h]
  float v83; // [esp+48h] [ebp-218h]
  float v84; // [esp+4Ch] [ebp-214h]
  _DWORD *v85; // [esp+4Ch] [ebp-214h]
  float v86; // [esp+50h] [ebp-210h]
  NiAVObject *v87; // [esp+50h] [ebp-210h]
  float v88; // [esp+54h] [ebp-20Ch]
  void *v89; // [esp+54h] [ebp-20Ch]
  MagicTarget *v90; // [esp+54h] [ebp-20Ch]
  NiPoint3 a3; // [esp+58h] [ebp-208h] BYREF
  float v92; // [esp+64h] [ebp-1FCh]
  TESChildCELL *v93; // [esp+68h] [ebp-1F8h]
  int v94; // [esp+6Ch] [ebp-1F4h]
  float *v95; // [esp+70h] [ebp-1F0h]
  NiPoint3 a4; // [esp+74h] [ebp-1ECh] BYREF
  int v97; // [esp+80h] [ebp-1E0h] BYREF
  NiTransform a2; // [esp+84h] [ebp-1DCh] BYREF
  unsigned int v99; // [esp+C0h] [ebp-1A0h]
  float v100; // [esp+C4h] [ebp-19Ch]
  __m128 v101; // [esp+D0h] [ebp-190h] BYREF
  __m128 v102; // [esp+E0h] [ebp-180h] BYREF
  int v103; // [esp+F0h] [ebp-170h]
  int v104; // [esp+F4h] [ebp-16Ch]
  __m128 v105[4]; // [esp+100h] [ebp-160h] BYREF
  __m128 v106[3]; // [esp+140h] [ebp-120h] BYREF
  __m128 v107; // [esp+170h] [ebp-F0h] BYREF
  __m128 v108[4]; // [esp+180h] [ebp-E0h] BYREF
  __m128 v109[4]; // [esp+1C0h] [ebp-A0h] BYREF
  __m128 v110[4]; // [esp+200h] [ebp-60h] BYREF
  unsigned int v111; // [esp+25Ch] [ebp-4h]
  NiPoint3 v112; // 0:^10.12

  v95 = arg4; /*0x60b174*/
  v93 = arg8; /*0x60b178*/
  v6 = (ArrowProjectile_CollisionData *)FormHeapAlloc(0x54u); /*0x60b180*/
  this->unk05C = v6; /*0x60b185*/
  LODWORD(v6->unk00[0]) = 1; /*0x60b188*/
  this->unk05C->unk2C[0] = 0.0; /*0x60b193*/
  this->unk05C->ninode = 0; /*0x60b199*/
  unk05C = this->unk05C; /*0x60b19c*/
  unk05C->unk00[4] = *arg4; /*0x60b1a1*/
  unk05C->unk00[5] = arg4[1]; /*0x60b1a7*/
  unk05C->unk00[6] = arg4[2]; /*0x60b1ad*/
  v8 = this->unk05C; /*0x60b1b5*/
  LODWORD(v8->unk00[1]) = *arg0; /*0x60b1b8*/
  LODWORD(v8->unk00[2]) = arg0[1]; /*0x60b1be*/
  LODWORD(v8->unk00[3]) = arg0[2]; /*0x60b1c7*/
  qmemcpy(&this->unk05C->unk2C[1], &stru_B26AF0[0xA].unk2C, 0x24u); /*0x60b1da*/
  v9 = &this->unk05C->unk00[7]; /*0x60b1e5*/
  *v9 = Vector3_InitValue_; /*0x60b1e8*/
  v9[1] = *(&Vector3_InitValue_ + 1); /*0x60b1f0*/
  v9[2] = dword_B3F9B0; /*0x60b1fe*/
  if ( MobileObject_GetCharProxy(&this->super) ) /*0x60b201*/
  {
    CharProxy = MobileObject_GetCharProxy(&this->super); /*0x60b210*/
    if ( CharProxy && (v11 = *((char **)CharProxy + 2)) != 0 ) /*0x60b21e*/
      v12 = (float *)sub_8AC0A0(v11); /*0x60b222*/
    else
      v12 = (float *)&stru_BA7A40; /*0x60b229*/
    v13 = *(_OWORD *)v12; /*0x60b22e*/
    a2.rot.data[2][1] = *v12; /*0x60b231*/
    v14 = flt_A7DEB4; /*0x60b23b*/
    *(_OWORD *)&a2.rot.data[1][0] = v13; /*0x60b241*/
    if ( -v14 == a2.rot.data[2][1] ) /*0x60b24f*/
    {
      if ( this->super.vtbl->super.GetNiNode(this) ) /*0x60b25f*/
      {
        GetNiNode = this->super.vtbl->super.GetNiNode; /*0x60b272*/
        speed = this->speed; /*0x60b278*/
        v86 = speed * *(float *)&dword_B258DC; /*0x60b286*/
        v92 = speed * *(float *)&qword_B258E0; /*0x60b292*/
        v84 = speed * *((float *)&qword_B258E0 + 1); /*0x60b29c*/
        a3.x = v86; /*0x60b2a4*/
        a3.y = v92; /*0x60b2ac*/
        a3.z = v84; /*0x60b2b4*/
        v17 = (int)GetNiNode((TESObjectREFR *)this); /*0x60b2b8*/
        v18 = sub_7101F0((NiTransform *)(v17 + 0x64), &a2, &a3); /*0x60b2c7*/
        v19 = &this->unk05C->unk00[7]; /*0x60b2d1*/
        *v19 = v18->rot.data[0][0]; /*0x60b2d4*/
        v19[1] = v18->rot.data[0][1]; /*0x60b2d9*/
        v19[2] = v18->rot.data[0][2]; /*0x60b2df*/
      }
    }
    else
    {
      sub_43F3E0(&this->unk05C->unk00[7], (__m128 *)a2.rot.data[1]); /*0x60b2f0*/
    }
  }
  v20 = this->super.vtbl->super.GetNiNode; /*0x60b2fa*/
  this->unk060 = 1; /*0x60b302*/
  v85 = 0; /*0x60b309*/
  v21 = (int)v20((TESObjectREFR *)this); /*0x60b311*/
  v22 = v93; /*0x60b313*/
  v87 = (NiAVObject *)v21; /*0x60b319*/
  if ( v93 && v21 ) /*0x60b325*/
  {
    shooter = this->shooter; /*0x60b32b*/
    if ( shooter ) /*0x60b330*/
    {
      if ( shooter->vtbl->GetCombatController(shooter) ) /*0x60b33a*/
      {
        v24 = this->shooter; /*0x60b348*/
        v78 = (*((int (__thiscall **)(TESChildCELL *))v22->vtbl + 0x5D))(v22); /*0x60b351*/
        v25 = (int)v24->vtbl->GetCombatController(v24); /*0x60b35c*/
        sub_618120(v25, (char)v24, v78, 0.0); /*0x60b360*/
      }
    }
    this->unk05C->ninode = (NiNode *)v22; /*0x60b368*/
    a2a = &this->unk05C->ninode->members.super.m_localTransform.rot.data[1][2]; /*0x60b37b*/
    v26 = this->super.vtbl->super.GetBaseForm(this); /*0x60b384*/
    Script_AddEventToExtraScript(v26, a2a, 0x100);// RealArenaTraining: arrow OnHitWith event. EBX=ArrowProjectile, ESI=hit ref, mask=0x100. /*0x60b387*/
    if ( a5 ) /*0x60b395*/
    {
      v27 = (Atmosphere *)sub_47FA60(*(int **)(a5 + 8)); /*0x60b39b*/
      if ( v27 ) /*0x60b3a5*/
      {
        v28 = sub_452A60(v27); /*0x60b3a9*/
        if ( v28 ) /*0x60b3b0*/
        {
          LODWORD(this->unk05C->unk2C[0]) = v28; /*0x60b3b5*/
          v85 = (_DWORD *)a5; /*0x60b3b8*/
        }
      }
    }
    v29 = this->unk05C; /*0x60b3bc*/
    if ( !LODWORD(v29->unk2C[0]) ) /*0x60b3bf*/
    {
      LODWORD(v29->unk2C[0]) = (TESChildCELL)v22[0xF].vtbl; /*0x60b3c8*/
      v30 = sub_480340(LODWORD(this->unk05C->unk2C[0])); /*0x60b3d2*/
      if ( !v30 ) /*0x60b3de*/
      {
LABEL_33:
        FormHeapFree((unsigned int)this->unk05C); /*0x60b545*/
        this->unk05C = 0; /*0x60b551*/
        this->unk060 = 0; /*0x60b554*/
        return; /*0x60b557*/
      }
      v85 = *(_DWORD **)(v30 + 0x10); /*0x60b3e3*/
    }
    if ( v85 && (v31 = v85[2]) != 0 && (v32 = (int *)(v31 + 0x14)) != 0 && (v33 = *v32) != 0 ) /*0x60b3ff*/
      v34 = *(float **)(v33 + 8); /*0x60b401*/
    else
      v34 = 0; /*0x60b41d*/
    if ( v85 ) /*0x60b421*/
      v35 = v85[2]; /*0x60b423*/
    else
      v35 = 0; /*0x60b428*/
    v36 = *(__m128 **)(v35 + 0x50); /*0x60b42a*/
    v105[0] = v36[1]; /*0x60b431*/
    v105[1] = v36[2]; /*0x60b43d*/
    v105[2] = v36[3]; /*0x60b449*/
    v105[3] = v36[4]; /*0x60b465*/
    sub_5398E0((int)v109, (float *)&v87->members.m_worldTransform); /*0x60b46d*/
    sub_8B1FF0(v108, v105, v109); /*0x60b48c*/
    v100 = 1.0; /*0x60b493*/
    a2.rot.data[1][0] = 0.0; /*0x60b4a1*/
    a2.rot.data[1][1] = flt_A6F3E0; /*0x60b4b4*/
    v103 = 0; /*0x60b4c0*/
    v104 = 0; /*0x60b4c7*/
    a2.rot.data[1][2] = 0.0; /*0x60b4ce*/
    v99 = 0xFFFFFFFF; /*0x60b4d2*/
    a2.rot.data[2][0] = 0.0; /*0x60b4dd*/
    a2.rot.data[2][1] = 0.0; /*0x60b4e1*/
    a2.rot.data[2][2] = flt_A6F3DC; /*0x60b4eb*/
    a2.pos.x = 0.0; /*0x60b4f2*/
    a2.pos.y = 0.0; /*0x60b4f9*/
    hkTransform_TransformPosition(&v101, v108, (__m128 *)a2.rot.data[1]); /*0x60b500*/
    hkTransform_TransformPosition(&v102, v108, (__m128 *)&a2.rot.data[2][1]); /*0x60b519*/
    sub_6077F0(v34, (int)&v101, (int)&a2.pos.z); /*0x60b530*/
    if ( v100 >= 1.0 ) /*0x60b543*/
      goto LABEL_33; /*0x60b543*/
    v37 = v34[4]; /*0x60b564*/
    v92 = v37; /*0x60b567*/
    if ( v99 != 0xFFFFFFFF ) /*0x60b56b*/
    {
      v38 = (*(int (__thiscall **)(float *))(*(_DWORD *)v34 + 0x88))(v34); /*0x60b577*/
      if ( v38 ) /*0x60b57b*/
      {
        v92 = COERCE_FLOAT((*(int (__thiscall **)(int, unsigned int))(*(_DWORD *)v38 + 0x9C))(v38, v99)); /*0x60b591*/
        v37 = v92; /*0x60b595*/
      }
      else
      {
        PrintError("The arrow raycast has returned a sub-shape key, but the shape was unable to find a shape collection"); /*0x60b59e*/
      }
    }
    v39 = this->unk05C; /*0x60b5a6*/
    v40 = v39->unk00[8]; /*0x60b5a9*/
    v39 = (ArrowProjectile_CollisionData *)((char *)v39 + 0x1C); /*0x60b5ac*/
    v41 = v39->unk00[0] * v39->unk00[0]; /*0x60b5bc*/
    v42 = v39->unk00[2] * v39->unk00[2]; /*0x60b5c0*/
    v81 = v40 * v40 + v41 + v42; /*0x60b5c4*/
    v82 = sqrt(v81); /*0x60b5d1*/
    sub_609D50(this, v41, v82, v82, *arg0, arg0[1], arg0[2], (int)v85, SLOBYTE(v37)); /*0x60b606*/
    v88 = -this->unk088; /*0x60b617*/
    v83 = -this->unk08C; /*0x60b623*/
    *(float *)&v94 = -this->unk090; /*0x60b62f*/
    a3.x = v88; /*0x60b637*/
    a3.y = v83; /*0x60b63f*/
    a3.z = *(float *)&v94; /*0x60b647*/
    sub_43F350(&a3.x); /*0x60b64b*/
    a3.x = a3.x + *v95; /*0x60b660*/
    a3.y = v95[1] + a3.y; /*0x60b66b*/
    a3.z = v95[2] + a3.z; /*0x60b676*/
    v43 = sub_43F350(&a3.x); /*0x60b67a*/
    v44 = (const char *)sub_5361B0(SLODWORD(v37)); /*0x60b687*/
    if ( v44 ) /*0x60b68e*/
    {
      TESObjectREFR_GetParentCell((TESObjectREFR *)this); /*0x60b696*/
      a2b = sub_4C9BE0((TESObjectREFR *)this); /*0x60b6a6*/
      ParentCell = TESObjectREFR_GetParentCell((TESObjectREFR *)this); /*0x60b6a9*/
      *(float *)&v94 = COERCE_FLOAT(sub_441800(ParentCell, a2b, 3u)); /*0x60b6b7*/
      v89 = (void *)FormHeapAlloc(0x20u); /*0x60b6c3*/
      v111 = 0; /*0x60b6c9*/
      if ( v89 ) /*0x60b6d4*/
      {
        v43 = flt_A31E2C; /*0x60b6e8*/
        v75 = *arg0; /*0x60b6ee*/
        v76 = arg0[1]; /*0x60b6f3*/
        v77 = (const char *)arg0[2]; /*0x60b6fa*/
        v112 = a3; /*0x60b706*/
        v74 = v94; /*0x60b714*/
        v73 = flt_A31E2C; /*0x60b71b*/
        v46 = TESObjectREFR_GetParentCell((TESObjectREFR *)this); /*0x60b71e*/
        v47 = sub_5713F0( /*0x60b728*/
                v89,
                (int)v46,
                v73,
                v74,
                v44,
                v112.x,
                v112.y,
                SLODWORD(v112.z),
                *(float *)&v75,
                v76,
                v77,
                1.0,
                0);
      }
      else
      {
        v47 = 0; /*0x60b72f*/
      }
      v111 = 0xFFFFFFFF; /*0x60b737*/
      sub_678D30((int *)&ActorProcessManager_ptr, (volatile LONG *)v47); /*0x60b742*/
    }
    v48 = this->shooter; /*0x60b747*/
    if ( v48 ) /*0x60b74c*/
    {
      *(float *)&v49 = COERCE_FLOAT((int)v48->members.magicCaster.vtbl->GetActiveMagicItem(&v48->members.magicCaster)); /*0x60b75b*/
      p_magicCaster = &this->shooter->members.magicCaster; /*0x60b760*/
      v94 = v49; /*0x60b763*/
      v90 = (MagicTarget *)((int (__thiscall *)(MagicCaster *))p_magicCaster->vtbl->GetCastingTarget)(p_magicCaster); /*0x60b76e*/
      arrowEnch = this->arrowEnch; /*0x60b772*/
      if ( arrowEnch ) /*0x60b777*/
      {
        this->shooter->members.magicCaster.vtbl->SetActiveMagicItem( /*0x60b789*/
          &this->shooter->members.magicCaster,
          (EnchantmentItem *)((char *)arrowEnch + 0x18));
        p_SetCastingTarget = &this->shooter->members.magicCaster.vtbl->SetCastingTarget; /*0x60b79d*/
        v53 = (*((int (__thiscall **)(TESChildCELL *))v93->vtbl + 0x49))(v93); /*0x60b7a0*/
        (*p_SetCastingTarget)(&this->shooter->members.magicCaster, (MagicTarget *)v53); /*0x60b7ab*/
        v54 = this->super.vtbl->super.GetBaseForm(this); /*0x60b7b7*/
        MagicCaster_UseActiveMagicItem(&this->shooter->members.magicCaster.vtbl, v41, v43, v42, (int)v54); /*0x60b7c0*/
      }
      poison = this->poison; /*0x60b7c5*/
      if ( poison ) /*0x60b7cd*/
      {
        this->shooter->members.magicCaster.vtbl->SetActiveMagicItem( /*0x60b7de*/
          &this->shooter->members.magicCaster,
          (AlchemyItem *)((char *)poison + 0x24));
        v56 = &this->shooter->members.magicCaster.vtbl->SetCastingTarget; /*0x60b7f2*/
        v57 = (*((int (__thiscall **)(TESChildCELL *))v93->vtbl + 0x49))(v93); /*0x60b7f5*/
        (*v56)(&this->shooter->members.magicCaster, (MagicTarget *)v57); /*0x60b800*/
        v58 = this->super.vtbl->super.GetBaseForm(this); /*0x60b80c*/
        MagicCaster_UseActiveMagicItem(&this->shooter->members.magicCaster.vtbl, v41, v43, v42, (int)v58); /*0x60b815*/
      }
      bowEnch = this->bowEnch; /*0x60b81a*/
      if ( bowEnch ) /*0x60b822*/
      {
        this->shooter->members.magicCaster.vtbl->SetActiveMagicItem( /*0x60b833*/
          &this->shooter->members.magicCaster,
          (EnchantmentItem *)((char *)bowEnch + 0x18));
        v60 = &this->shooter->members.magicCaster.vtbl->SetCastingTarget; /*0x60b847*/
        v61 = (*((int (__thiscall **)(TESChildCELL *))v93->vtbl + 0x49))(v93); /*0x60b84a*/
        (*v60)(&this->shooter->members.magicCaster, (MagicTarget *)v61); /*0x60b855*/
        v62 = this->super.vtbl->super.GetBaseForm(this); /*0x60b861*/
        MagicCaster_UseActiveMagicItem(&this->shooter->members.magicCaster.vtbl, v41, v43, v42, (int)v62); /*0x60b86a*/
      }
      this->shooter->members.magicCaster.vtbl->SetActiveMagicItem(&this->shooter->members.magicCaster, (MagicItem *)v94); /*0x60b87f*/
      this->shooter->members.magicCaster.vtbl->SetCastingTarget(&this->shooter->members.magicCaster, v90); /*0x60b892*/
    }
    v63 = v93; /*0x60b894*/
    v64 = TESObjectREFR_GetParentCell((TESObjectREFR *)v93); /*0x60b89a*/
    if ( v64 && (sub_4440C0(v64), v65) ) /*0x60b8ac*/
      v66 = *(_DWORD **)(v65 + 0x24); /*0x60b8ae*/
    else
      v66 = 0; /*0x60b8b3*/
    if ( v66 ) /*0x60b8b7*/
    {
      if ( sub_536AE0(v66, (int)v63) ) /*0x60b8ba*/
      {
        if ( v63 != (TESChildCELL *)0xFFFFFFBC ) /*0x60b8c8*/
          Script_AddEventToExtraScript(v63, &v63[0x11], 0x10000000); /*0x60b8d1*/
      }
    }
    if ( this->arrowEnch ) /*0x60b8d9*/
      this->unk060 = 3; /*0x60b8df*/
    p_m_worldTransform = &v87->members.m_worldTransform; /*0x60b8f3*/
    v68 = sub_7101F0(&v87->members.m_worldTransform, &a2, (NiPoint3 *)&dword_B258DC); /*0x60b8f9*/
    sub_6088F0( /*0x60b92f*/
      (float *)this,
      *(float *)arg0,
      *((float *)arg0 + 1),
      *((float *)arg0 + 2),
      v68->rot.data[0][0],
      v68->rot.data[0][1],
      v68->rot.data[0][2],
      v85);
    switch ( LODWORD(v92) ) /*0x60b944*/
    {
      case 0: /*0x60b944*/
      case 3: /*0x60b944*/
      case 5: /*0x60b944*/
      case 0xA: /*0x60b944*/
      case 0xB: /*0x60b944*/
      case 0xD: /*0x60b944*/
      case 0xF: /*0x60b944*/
      case 0x12: /*0x60b944*/
      case 0x14: /*0x60b944*/
      case 0x19: /*0x60b944*/
      case 0x1A: /*0x60b944*/
      case 0x1C: /*0x60b944*/
      case 0x1E: /*0x60b944*/
        goto LABEL_66;
      default:
        v69 = *sub_497340(v85, &v97) & 0x3F; /*0x60b965*/
        if ( v69 && (v69 <= 3 || v69 > 6) ) /*0x60b976*/
        {
          a4.x = 0.0; /*0x60b982*/
          a4.y = v100 * dbl_A687B0 - dbl_A3F428; /*0x60b9a1*/
          a4.z = 0.0; /*0x60b9a5*/
          a4.y = a4.y + dbl_A2F910; /*0x60b9b3*/
          sub_53D4B0(p_m_worldTransform, &a3.x, &a4); /*0x60b9b7*/
          x = a3.x; /*0x60b9bc*/
          TESObjectREFR_SetPosition((TESObjectREFR *)this, a3.x, a3.y, a3.z); /*0x60b9d7*/
          y = a3.y; /*0x60b9e2*/
          z = a3.z; /*0x60b9e6*/
          v87->members.m_localTransform.pos.x = x; /*0x60b9ec*/
          v87->members.m_localTransform.pos.y = y; /*0x60b9f0*/
          v87->members.m_localTransform.pos.z = z; /*0x60b9f6*/
          NiAVObject_UpdateNiAVObject(v87, 0.0, 0); /*0x60b9f9*/
          sub_5398E0((int)v110, (float *)p_m_worldTransform); /*0x60ba07*/
          sub_8B1FF0(v106, v105, v110); /*0x60ba26*/
          sub_43F3E0(&this->unk05C->unk00[1], &v107); /*0x60ba3a*/
          sub_607740((int)&this->unk05C->unk2C[1], v106); /*0x60ba4e*/
        }
        else
        {
LABEL_66:
          sub_608DA0(&this->super, (int)arg0, (int)v95); /*0x60ba58*/
        }
        break; /*0x60ba56*/
    }
  }
}
