lweapon script exploderang
{
	void run(int explosion_damage)
	{
		while(this->isValid)
		{
			for ( int q = Screen->NumNPCs(); q > 0; --q )
			{
				npc n = Screen->LoadNPC(q);
				if ( n->Defense[NPCD_BOOMERANG] != NPCDT_IGNORE && Collision(this, n) )
				{
					n->InvFrames = 0;
					lweapon boom = Screen->CreateLWeapon(LW_BOMBBLAST);
					boom->X = this->X;
					boom->Y = this->Y;
					boom Dir = this->Dir;
					boom->Step = 0;
					boom->Damage = ( explosion_damage > 0 ) ? explosion_damage : this->Power;
					this->DeadState = WDS_DEAD;
					Remove(this);
				}
			}
			Waitframe();
		}
	}
}
					