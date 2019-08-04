#pragma once
#include "Gear.h"
#include "Parsable.h"


class LootTable : public Parsable
{
private:
	void pv_parseStep(std::vector<std::string>)override;
	std::vector<GearPiece*> m_lootEntries;
	std::vector<GearPiece*> m_toDeleteGearPieces;
	bool m_isEmpty = true;
public:
	void addLootEntry(const GearPiece&, unsigned int incidence = 1);

	bool isEmpty()const;
	void mergeLootTable(const LootTable&);

	GearPiece getRandomGearPiece();
	~LootTable();
};

