#pragma once
#include <vector>
#include <queue>
#include <map>
#include "Serializable.h"
#include "Gear.h"
#include "Menu.h"
#include "ToolTip.h"


//TODO: finish UI
class PlayerInventory : public Menu, public Serializable
{
private:
	std::map<gearTypes, GearPiece> m_playerGear;
	std::vector<inventoryItem> m_items;

	ToolTip *m_toDrawToolTip = nullptr;

	size_t m_maxColumns = 10;
	size_t m_maxRows = 5;
	void pv_onClick(MenuItem*, size_t, const InputManager&, MessageBus*)override;
	//use only when sure the gear piece in the slot wont be detroyed but insted switched
	void m_addGearToPlayer(const GearPiece&);
public:

	using Menu::Menu;

	void createFrom(const decomposedData&)override;
	decomposedData serialize()override;


	inventoryItem removeItem(size_t);

	void removeAmmo(size_t);
	void addAmmo(size_t);

	//please include a texture to display
	size_t addItemToInventory(inventoryItem&);
	size_t addItemToInventory(const GearPiece&);
	inventoryItem equipInventoryItem(inventoryItem);

	void onDraw(bool, sf::Vector2f)override;
	void createStaticMenuLayout()override;

	void update(updateEvent)override;

	~PlayerInventory();
};

