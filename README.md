# CSSv90 SteamID Spoofer
**CSSv90 SteamID Spoofer** is a small library that allows you to set your own SteamID value in **Counter-Strike: Source v90 No-Steam** game.

# Features
As mentioned above - with this module you have the opportunity to change your SteamID to any desired. However, in addition to this, the module also fixes the **RevEmu2013** emulator problem, due to which the game may not start on some operating systems.

# How to use?
First you need to inject the module into the game. It is important to inject immediately after launching the game, otherwise stable work of spoofer cannot be guaranteed. For an injection, you can use any existing library injector.

The module registers two console variables - **sid_value** and **sid_shift**.

With console variable **sid_value** you can set the desired SteamID. If the value is set to **0**, then at each connection to game server a new random SteamID will be generated.

The console variable **sid_shift** is needed to change the behavior of the SteamID generation algorithm. On some servers there is a chance to get the value of SteamID divided by 2. By setting the value of this variable to **0**, you have the chance to fix this problem.

# License
This project is licensed under the MIT License.