/* message.h */

#ifndef MESSAGE_H
#define MESSAGE_H


#define assert_pointer(ptr, ...) \
if (ptr == NULL) \
{ \
    log_info(__VA_ARGS__); \
    exit(-1); \
}

#define verify_pointer(ptr, ...) \
if (ptr == NULL) \
{ \
    log_info(__VA_ARGS__); \
    return; \
}

typedef unsigned long int u64snowflake;

void copy_message(
    struct discord *client,
    const struct discord_message *base_message,
    u64snowflake to_channel,
    char *edits);

void move_message(
    struct discord *client,
    const struct discord_message *base_message, 
    u64snowflake to_channel);

bool is_ignorable(char character);

char *format_content(char *base_content);

void handle_message_commands(struct discord *client, const struct discord_message *message);

#endif //MESSAGE_H