#include "PlayerInventory.h"
#include "Animator.h"




PlayerInventory::~PlayerInventory()
{
}

void PlayerInventory::removeAmmo(size_t amount)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].itemType == ammoType) {
			if (amount >= m_items[i].amount) {
				amount -= m_items[i].amount;
				m_items[i].amount = 1;
				m_items[i].itemType = emptySlot;
				m_menuItems[i]->setIsButtonTextDisplayed(false);
				if (amount == 0) {
					return;
				}
				continue;
			}
			else {
				m_items[i].amount -= amount;
				m_menuItems[i]->setButtonText(std::to_string(m_items[i].amount), 0.1, sf::Color::White, 0);
				return;
			}
		}
	}
}

void PlayerInventory::addAmmo(size_t amount)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].itemType == ammoType) {
			m_items[i].amount += amount;
			m_menuItems[i]->setButtonText(std::to_string(m_items[i].amount), 0.1, sf::Color::White, 0);
			return;
		}
	}
	inventoryItem tempInvItem;
	tempInvItem.itemType = ammoType;
	tempInvItem.itemASprite.textureID = Animator::getInstance().getTextureID("bullet.png");
	tempInvItem.amount = amount;
	size_t slotSelected = addItemToInventory(tempInvItem);
	m_menuItems[slotSelected]->setButtonText(std::to_string(m_items[slotSelected].amount), 0.1, sf::Color::White, 0);
}

size_t PlayerInventory::addItemToInventory(inventoryItem& item)
{
	if (item.itemType == emptySlot) {
		throw "trying to add an empty inventoryItem object to inventory!";
	}
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].itemType == emptySlot) {
			item.itemASprite.isUI = true;
			m_menuItems[i]->fitASpriteToItem(&item.itemASprite);
			//item.itemASprite.position = m_menuItems[i].getPosition();
			

			m_items[i] = item;
			return i;
		}
	}
}

size_t PlayerInventory::addItemToInventory(const GearPiece& gearPiece)
{
	inventoryItem tempInventoryItem;
	tempInventoryItem.simpleRep = gearPiece;
	tempInventoryItem.itemType = gearPieceType;
	tempInventoryItem.itemASprite = gearPiece.tex;
	return addItemToInventory(tempInventoryItem);
}

void PlayerInventory::onDraw(bool beforeDraw, sf::Vector2f displacement)
{
	if (!beforeDraw) {

		for (size_t i = 0; i < m_items.size(); i++)
		{
			if (m_items[i].itemType != emptySlot) {
				//m_items[i].itemASprite.position += displacement;
				m_items[i].itemASprite.UIDisplacement = displacement;
				Animator::getInstance().instantDraw(m_items[i].itemASprite);
				//m_items[i].itemASprite.position -= displacement;
			}
		}
		for (auto &x : m_playerGear)
		{
			//x.second.tex.position += displacement;
			x.second.tex.UIDisplacement = displacement;
			Animator::getInstance().instantDraw(x.second.tex);
			//x.second.tex.position -= displacement;
		}
		if (m_toDrawToolTip != nullptr) {
			m_toDrawToolTip->setPosition(m_toDrawToolTip->getPosition());
			m_toDrawToolTip->setViewDisplacement(displacement);
			m_toDrawToolTip->draw(*m_window);
			m_toDrawToolTip = nullptr;
		}
		
	}
}

void PlayerInventory::createStaticMenuLayout()
{

	sf::Vector2f invStart = sf::Vector2f(0.35, 0.35);
	sf::Vector2f menuItemSize = sf::Vector2f(0.06, 0.06);

	for (size_t i = 0; i < m_maxColumns; i++)
	{
		for (size_t o = 0; o < m_maxRows; o++)
		{
			//std::cout << i << ", "<< m_maxColumns << std::endl;
			//std::cout << float(i) / m_maxColumns << std::endl;
			inventoryItem tempInventoryItem;
			tempInventoryItem.itemType = emptySlot;
			m_items.push_back(tempInventoryItem);
			MenuItem *tempMenuItem = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(float(i)*menuItemSize.x + invStart.x,float(o)*menuItemSize.y + invStart.y),true), getPixelCoordinate(menuItemSize)));
			AnimatorSprite tempASprite;
			tempASprite.textureID = Animator::getInstance().getTextureID("Inventory-Slot (Empty).png");
			tempMenuItem->setTexture(tempASprite);
			tempMenuItem->setHasClickTimer(false);
			addMenuItem(tempMenuItem);
		}
	}
	//helmet m_maxColumns*m_maxRows + 1
	MenuItem *helmetSlot = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(invStart.x-menuItemSize.x, invStart.y),true), getPixelCoordinate(menuItemSize)));
	AnimatorSprite helmetSlotASprite;
	helmetSlotASprite.textureID = Animator::getInstance().getTextureID("Inventory-Helmet.png");
	helmetSlot->setTexture(helmetSlotASprite);
	addMenuItem(helmetSlot);

	//chestPiece
	MenuItem *chestpieceSlot = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(invStart.x- menuItemSize.x, invStart.y + menuItemSize.y),true), getPixelCoordinate(menuItemSize)));
	AnimatorSprite chestPieceSlotASprite;
	chestPieceSlotASprite.textureID = Animator::getInstance().getTextureID("Inventory-Chest Armor.png");
	chestpieceSlot->setTexture(chestPieceSlotASprite);
	addMenuItem(chestpieceSlot);


	MenuItem *jumpSuitSlot = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(invStart.x- menuItemSize.x, invStart.y + menuItemSize.y*2),true), getPixelCoordinate(menuItemSize)));
	AnimatorSprite jumpSuitSlotASprite;
	jumpSuitSlotASprite.textureID = Animator::getInstance().getTextureID("Inventory-Jumpsuit.png");
	jumpSuitSlot->setTexture(jumpSuitSlotASprite);
	addMenuItem(jumpSuitSlot);


	MenuItem *bootsSlot = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(invStart.x- menuItemSize.x, invStart.y + menuItemSize.y*3),true), getPixelCoordinate(menuItemSize)));
	AnimatorSprite bootsSlotASprite;
	bootsSlotASprite.textureID = Animator::getInstance().getTextureID("Inventory-Boots.png");
	bootsSlot->setTexture(bootsSlotASprite);
	addMenuItem(bootsSlot);

	//weapon
	MenuItem *weaponSlot = new MenuItem(sf::FloatRect(getPixelCoordinate(sf::Vector2f(0.05, 0.55),true), getPixelCoordinate(menuItemSize)));

	addMenuItem(weaponSlot);

}

void PlayerInventory::update(updateEvent)
{
}

void PlayerInventory::pv_onClick(MenuItem *clickedMenuItem, size_t clickedMenuItemIndex, const InputManager &inputs, MessageBus *gameBus)
{

	if (inputs.isInputEventActive(InputManager::shoot)) {
		//std::cout << clickedMenuItemIndex << ", " << m_maxColumns * m_maxRows << std::endl;
		m_menuItems[clickedMenuItemIndex]->clearBehaviours();

		


		if (clickedMenuItemIndex >= m_maxColumns * m_maxRows) {
			if (m_playerGear.find(gearTypes(clickedMenuItemIndex - (m_maxColumns * m_maxRows))) != m_playerGear.end()) {
				//send behviour indicating the clicked gear piece must be unequipped
				behaviourParameters tempBParam;
				tempBParam.behaviourName = removesGearPiece;
				tempBParam.gearPiece = m_playerGear[gearTypes(clickedMenuItemIndex - (m_maxColumns * m_maxRows))];
				m_menuItems[clickedMenuItemIndex]->addBehviourParameters(tempBParam);


				//add equipped item to inventory
				addItemToInventory(m_playerGear[gearTypes(clickedMenuItemIndex - (m_maxColumns * m_maxRows))]);
				//

				//erase gear piece from playerGear
				m_playerGear.erase(gearTypes(clickedMenuItemIndex - (m_maxColumns * m_maxRows)));
			}

			return;
		}
		if (m_items[clickedMenuItemIndex].itemType == gearPieceType) {
			
			
			//check if item type is currently equipped
			if (m_playerGear.find(m_items[clickedMenuItemIndex].simpleRep.type) == m_playerGear.end()) {
				//gear type not currently equipped
				//just equip new item
				m_addGearToPlayer(m_items[clickedMenuItemIndex].simpleRep);
				//send behaviour indicating that it must be equipped
				behaviourParameters tempBParam;
				tempBParam.behaviourName = equipsGearPiece;
				tempBParam.gearPiece = m_playerGear[m_items[clickedMenuItemIndex].simpleRep.type];
				m_menuItems[clickedMenuItemIndex]->addBehviourParameters(tempBParam);
				//remove the newly equipped item from inventory
				m_items[clickedMenuItemIndex].itemType = emptySlot;
			
			}
			else {
				//gear type already equipped
				//send behviour indicating it must be unequipped
				behaviourParameters tempBParam;
				tempBParam.behaviourName = removesGearPiece;
				tempBParam.gearPiece = m_playerGear[m_items[clickedMenuItemIndex].simpleRep.type];
				m_menuItems[clickedMenuItemIndex]->addBehviourParameters(tempBParam);
				//


				//add already equipped item to inventory and remove newly equipped item from inventory
				addItemToInventory(m_playerGear[m_items[clickedMenuItemIndex].simpleRep.type]);
				m_items[clickedMenuItemIndex].itemType = emptySlot;
				
		
				//add newly equippoed GearPiece to equipped gear
				m_addGearToPlayer(m_items[clickedMenuItemIndex].simpleRep);
				//send behaviour indicating it must equipped
				tempBParam.behaviourName = equipsGearPiece;
				tempBParam.gearPiece = m_items[clickedMenuItemIndex].simpleRep;
				m_menuItems[clickedMenuItemIndex]->addBehviourParameters(tempBParam);
			
			}
		
		}
	}
	else {
		if (clickedMenuItemIndex < (m_maxColumns * m_maxRows)) {
			if (m_items[clickedMenuItemIndex].itemType == gearPieceType) {
				m_items[clickedMenuItemIndex].itemToolTip.makeTooltipForGear(m_items[clickedMenuItemIndex].simpleRep);
				AnimatorSprite tempASprite;
				tempASprite.scale = 5;
				tempASprite.originToCenter = false;
				tempASprite.textureID = Animator::getInstance().getTextureID("tooltip.png");
				m_items[clickedMenuItemIndex].itemToolTip.setTexture(tempASprite);
				m_items[clickedMenuItemIndex].itemToolTip.setPosition(m_menuItems[clickedMenuItemIndex]->getPosition() + m_menuItems[clickedMenuItemIndex]->getDimension());
				m_toDrawToolTip = &m_items[clickedMenuItemIndex].itemToolTip;
				m_toDrawToolTip->setIsUIElement(true);
			}
		}


	}
	if (inputs.isInputEventActive(InputManager::dropItem)) {
		

		if (m_items[clickedMenuItemIndex].itemType == gearPieceType) {

			//spawn the item first
			/*behaviourParameters tempDropParam;
			tempDropParam.behaviourName = dropsItemOnPlayer;
			tempDropParam.itemDropped = m_items[clickedMenuItemIndex];
			m_menuItems[clickedMenuItemIndex].addBehviourParameters(tempDropParam);*/
			//then remove from inventory



			//eventually will make it so the item drops instead of disappearing--done
			MessageData *tempMessageData = new MessageData();
			tempMessageData->messageType = "spawnItemOnPlayer";
			tempMessageData->messageContents.push_back(m_items[clickedMenuItemIndex].serialize());
			gameBus->addMessage(tempMessageData);

			m_items[clickedMenuItemIndex].itemType = emptySlot;

		}


		//message bound item dropping test
		/*behaviourParameters tempBParam;
		tempBParam.behaviourName = sendsMessage;
		tempBParam.messageData.messageType = "spawnItemOnPlayer";
		tempBParam.messageData.messageContents.push_back(m_items[clickedMenuItemIndex].serialize());
		m_menuItems[clickedMenuItemIndex].addBehviourParameters(tempBParam);*/

	}
	
}

void PlayerInventory::m_addGearToPlayer(const GearPiece& gearPiece)
{
	size_t tempMenuID = m_maxColumns * m_maxRows + gearPiece.type;
	m_playerGear[gearPiece.type] = gearPiece;
	m_playerGear[gearPiece.type].tex.isUI = true;
	m_menuItems[tempMenuID]->fitASpriteToItem(&m_playerGear[gearPiece.type].tex);
	
}

void PlayerInventory::createFrom(const decomposedData& dData)
{
	decomposedData tempDData = dData;
	for (size_t i = 0; i < tempDData.childrenObjects.size(); i++)
	{
		if (tempDData.childrenObjects[i].type == "inventoryItem") {
			inventoryItem tempInventoryItem;
			tempInventoryItem.createFrom(tempDData.childrenObjects[i]);
			if (tempInventoryItem.itemType != emptySlot) {
				addItemToInventory(tempInventoryItem);
			}
			
			
		}
	}
}

decomposedData PlayerInventory::serialize()
{
	decomposedData retDData;
	retDData.name = "lastPlayerInventory";
	retDData.type = "PlayerInventory";
	for (size_t i = 0; i < m_items.size(); i++)
	{
		retDData.addChildrenObject(m_items[i].serialize());
	}

	return retDData;
}
