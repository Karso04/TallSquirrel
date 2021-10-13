#include "main.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}
//Make speedy on floor
MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(PlayerController_Start, "HexabodyVR.PlayerController", "HexaBodyPlayer3", "Start", void, Il2CppObject* self){
    PlayerController_Start(self);
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "RunSpeed", 1000.0f));
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self,"RunAcceleration", 1000.0f));
}
//Make speedy in air
MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(FlightController_Update, "", "FlightController", "Update", void, Il2CppObject* self){
    FlightController_Update(self);
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "maxVel", 250.0f));
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "maxVelocity", 250.0f));
}
// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    INSTALL_HOOK(getLogger(), PlayerController_Start);
    INSTALL_HOOK(getLogger(), FlightController_Update);
    getLogger().info("Installed all hooks!");
}