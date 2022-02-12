@@ -1,46 +1,59 @@
#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "<count> <mode: give / offhand : 1/0>") {
DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "") {
}

DupeCommand::~DupeCommand() {
}

bool DupeCommand::execute(std::vector<std::string>* args) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();

	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = inv->getItemStack(selectedSlot);
	C_ItemStack* selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

	auto slot = supplies->selectedHotbarSlot;

	int count = item->count;
	bool isGive = true;
	C_InventoryAction actions[36 * 2];
	int numActions = 0;
	for(int i = 0; i < 36; i++){
		if(i == slot)
			continue;
		auto itemInSlot = supplies->inventory->getItemStack(i);

	if (args->size() > 1)
		item->count = assertInt(args->at(1));
	if (args->size() > 2)
		isGive = static_cast<bool>(assertInt(args->at(2)));
		if(itemInSlot == nullptr || !itemInSlot->isValid() || itemInSlot->getItem()->itemId == 0)
		{
			actions[numActions] = C_InventoryAction(slot, selectedItem, nullptr, 0, 0);
			actions[numActions + 1] = C_InventoryAction(i, nullptr, selectedItem, 0, 0);

	if (isGive) {
		int slot = inv->getFirstEmptySlot();
			numActions += 2;
		}
	}

		C_InventoryAction* firstAction = nullptr;
		C_InventoryAction* secondAction = nullptr;
	C_InventoryTransactionPacket pk;
	pk.complexTransaction = new C_ComplexInventoryTransaction();
	pk.complexTransaction->actionType = 0;

		firstAction = new C_InventoryAction(0, item, nullptr, 507, 99999);
		secondAction = new C_InventoryAction(slot, nullptr, item);
	struct temp {
		__int64 next;
		char pad[0x18];
		C_InventoryAction* start;
		C_InventoryAction* end;
	};

		transactionManager->addInventoryAction(*firstAction);
		transactionManager->addInventoryAction(*secondAction);
	temp coolBean{};
	coolBean.start = &actions[0];
	coolBean.end = &actions[numActions];

		inv->addItemToFirstEmptySlot(item);
	} else
		g_Data.getLocalPlayer()->setOffhandSlot(item);
	auto ptrBean = &coolBean;

	if (args->size() > 1)
		item->count = count;
	coolBean.next = reinterpret_cast<__int64>(&ptrBean);

	clientMessageF("%sSuccessfully duplicated the item!", GREEN);
	pk.complexTransaction->transac.ptr = reinterpret_cast<__int64>(&ptrBean);
	pk.numTransactions = 1;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
	clientMessageF("%sSuccessfully duplicated item! (%i times)", GREEN, numActions / 2);
	return true;
}
