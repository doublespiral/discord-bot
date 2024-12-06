/* message.c */

// imports
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <concord/discord.h>
#include <concord/log.h>

#include "message.h"


// defines
#define send_message(str) \
{ \
    struct discord_create_message response = { .content = str }; \
    discord_create_message(client, message->channel_id, &response, NULL); \
}

void copy_message(
    struct discord *client,
    const struct discord_message *base_message,
    u64snowflake to_channel,
    char *edits)
{
    // copy message
    struct discord_create_message copied_message = {};

    copied_message.content = malloc( strlen(edits) + strlen(base_message->content) );

    verify_pointer(
        copied_message.content,
        "malloc skill issue for `copied_message.content`, returning early"
    );

    sprintf(copied_message.content, "%s%s", edits, base_message->content);

    copied_message.embeds = base_message->embeds;

    // send copied message
    discord_create_message(client, to_channel, &copied_message, NULL);

    free(copied_message.content);
}


void move_message(
    struct discord *client,
    const struct discord_message *base_message, 
    u64snowflake to_channel)
{
    copy_message(client, base_message, to_channel, "Moved message: \n");

    discord_delete_message(client, base_message->channel_id, base_message->id, NULL, NULL);
}


bool is_ignorable(char character)
{
    switch (character)
    {
        case ' ':
        case '.':
        case '\'':
        case '\"':
        return true;
    }
    return false;
}


char *format_content(char *base_content)
{
    char *formatted = malloc( strlen(base_content) );
    assert_pointer(formatted, "Malloc skill issue on `formatted`");

    size_t ignored = 0;
    size_t i;
    for (i = 0; base_content[i] != '\0'; i++)
    {
        unsigned char current = base_content[i];
        if ( is_ignorable(current) )
        {
            ignored++;
            continue;
        }
        formatted[i - ignored] = tolower( current );
    }
    formatted[i - ignored] = '\0';

    return formatted;
}


void handle_message_commands(struct discord *client, const struct discord_message *message)
{
    char *formatted = format_content(message->content);

    log_info("formatted: %s", formatted);

    if ( strstr(formatted, "lobster") != NULL )
    {
        // Wow!!! C is just like python!!!!
        if ( strstr(formatted, "enjoy") != NULL )
            send_message("((He actually means something else))")
    }

    else if ( strstr(formatted, "spotlight") != NULL )
        send_message("The lights keep you blinded")

    else if ( strstr(formatted, "dave") != NULL )
        send_message("I can't put down the cup")

    free( formatted );
}


