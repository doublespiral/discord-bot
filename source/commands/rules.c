/* rules.c */

//imports
#include <string.h>
#include <stdlib.h>

#include <concord/discord.h>
#include <concord/log.h>

#include "rules.h"

// defines
#define CHANNEL_RULES 1300928626392301750
#define MAX_RULE_NUM_DIGITS 24 // no way we're getting past rule 999999999999999999999999 right?


bool is_rule_formatted(struct discord *client, const struct discord_message *message)
{
    struct discord_messages messages = { 0 };
    struct discord_ret_messages ret = { .sync = &messages };

    // limit of 2 allows space for both the current and last sent message
    struct discord_get_channel_messages params = { .limit = 2 };
    discord_get_channel_messages(client, CHANNEL_RULES, &params, &ret);

    if (messages.size != 2)
    {
        log_info("Please send the first rule when the bot is offline");
        // Assume incorrectly formatted
        return false;
    }

    char *previous_rule = messages.array[1].content;
    if (previous_rule[0] == '\0')
    {
        log_info("Previous rule was blank for some reason, returning...");
        // Assume incorrectly formatted
        return false;
    }

    // we're checking this part of the prefix to see if we can return early before doing the more
    // expensive operations
    char prefix_p1[6 + 1] = "rule #";
    for (size_t i = 0; i<sizeof(prefix_p1); i++)
    {
        if (message->content[i] == '\0') return false;

        prefix_p1[i] = message->content[i];
    }

    if ( strcmp(prefix_p1, "rule #") == false ) return false;

    // check if the provided number is correct
    char previous_rule_number[MAX_RULE_NUM_DIGITS + 1] = "";

    for (
        size_t i = 0;
        i < (MAX_RULE_NUM_DIGITS + 1)
        && previous_rule[i+6] != ':'
        && previous_rule[i+6] != '\0';
        i++)
    {
        previous_rule_number[i] = previous_rule[i+6];
    }

    char prefix_p2[MAX_RULE_NUM_DIGITS + 3] = "";

    sprintf(prefix_p2, "%d: ", (1 + atoi(previous_rule_number)));

    // start as true so we can && it (itself) and (if the current char is correct)
    bool result = true;

    {
        // we need scope wide access to i to check if the last character is a space
        size_t i;
        for(
            i = 0; 
            i<MAX_RULE_NUM_DIGITS + 3
            // easiest way to check when to stop 
            && message->content[i+6] != ':'; 
            i++)
        {
            if (message->content[i+6] == '\0')
                return false;

            result = result && (prefix_p2[i] == message->content[i+6]);
        }
        // we check for the last space here
        result = result && (prefix_p2[i] == message->content[i+6]);
    }

    return result;
}


void enforce_rule_format(struct discord *client, const struct discord_message *message)
{
    if ( is_rule_formatted(client, message) == false )
    {
        discord_delete_message(client, message->channel_id, message->id, NULL, NULL);
    }
}