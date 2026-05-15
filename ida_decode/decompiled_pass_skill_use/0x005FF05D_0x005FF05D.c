// 0x005FF05D @ 0x005FF05D
int __userpurge Actor_AttackHandling_::DetermineTarget@<eax>(
        TESObjectREFR *a1@<ebx>,
        int a2@<ebp>,
        int *a3@<edi>,
        double a4@<st2>,
        double a5@<st1>,
        double a6@<st0>,
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
        int a17,
        int a18,
        int a19,
        float a20)
{
  TESObjectREFR *ActorWithinReach; // esi
  int v21; // ecx
  int v22; // eax

  if ( a1 ) /*0x5ff003*/
  {
    ActorWithinReach = a1; /*0x5ff005*/
  }
  else
  {
    a4 = a20; /*0x5ff009*/
    ActorWithinReach = (TESObjectREFR *)GetActorWithinReach__(a3, (int)a3, a20); /*0x5ff01a*/
  }
  if ( !ActorWithinReach ) /*0x5ff01e*/
  {
    if ( (a3 != (int *)TESDataHandler_g_PlayerRef || !sub_5F9620((TESObjectREFR *)a3, a5, a6)) && !a15 ) /*0x5ff06f*/
    {
      if ( a2 ) /*0x5ff077*/
        JUMPOUT(0x5FEC7D); /*0x5fec7d*/
      JUMPOUT(0x5FEC7A); /*0x5fec7a*/
    }
    return Actor_AttackHandling_::Done(a7, a8, a9);// RealArenaTraining: confirmed attack-target caller for player static probe. EDI must be player; no actor target was found; call enters sub_5F9620 for static/reach handling. /*0x5ff06f*/
  }
  if ( BaseExtraList_HasGhost(&ActorWithinReach->member.baseExtraList) ) /*0x5ff023*/
  {
    v21 = a3[0x16]; /*0x5ff02c*/
    if ( v21 ) /*0x5ff031*/
    {
      if ( (*(int (__thiscall **)(int, int))(*(_DWORD *)v21 + 0xEC))(v21, 1) ) /*0x5ff041*/
        JUMPOUT(0x5FEC6D); /*0x5fec6d*/
    }
    JUMPOUT(0x5FEC6B); /*0x5fec6b*/
  }
  if ( ActorWithinReach->vtbl->IsDead(ActorWithinReach, 0) ) /*0x5ff095*/
    return Actor_AttackHandling_::Done(a7, a8, a9); /*0x5ff064*/
  if ( !a15 ) /*0x5ff0a4*/
  {
    if ( (*(int (__thiscall **)(int *))(*a3 + 0x330))(a3) ) /*0x5ff0b0*/
    {
      v22 = (*(int (__thiscall **)(int *))(*a3 + 0x330))(a3); /*0x5ff0c1*/
      CombatController_TransmitDisease_(v22, (int)a1, a2, a4, ActorWithinReach); /*0x5ff0c5*/
    }
  }
  return Actor_AttackHandling_::___Check(
           (int)a3,
           (int)ActorWithinReach,
           a7,
           a8,
           a9,
           a10,
           a11,
           a12,
           a13,
           a14,
           a15,
           a16,
           a17,
           a18);
}
