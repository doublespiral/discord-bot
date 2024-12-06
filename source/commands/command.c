/* command.c */

// imports
#include <concord/discord.h>
#include <concord/discord-internal.h>

#include "command.h"
#include "message.h"
#include "rules.h"

// defines
#define CHANNEL_RIZZ_UNO   1311795119044689950
#define CHANNEL_RIZZ_DOS   1314355301172121600
#define CHANNEL_RULES      1300928626392301750
#define CHANNEL_TRASH_OUT  1302776301161287752
#define BOT_PANCAKE         239631525350604801


void on_message(struct discord *client, const struct discord_message *message)
{
    // prevent self recursion
    if (message->author->id == client->self.id) return;

    switch (message->channel_id)
    {
        case CHANNEL_RULES:
            enforce_rule_format(client, message);
            break;

        case CHANNEL_RIZZ_UNO:
            copy_message(client, message, CHANNEL_RIZZ_DOS, "");
            break;

        case CHANNEL_RIZZ_DOS:
            copy_message(client, message, CHANNEL_RIZZ_UNO, "");
            break;
    }

    if (message->author->id == BOT_PANCAKE)
    {
        move_message(client, message, CHANNEL_TRASH_OUT);
    }

    handle_message_commands(client, message);
}


//void sync_rizz()
//{
//    sync_messages_some
//}
//
//void sync_rizz_full();