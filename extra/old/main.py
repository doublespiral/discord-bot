from json import loads as load_json
from typing import Final

from discord import Intents, Client, Message



with open("secrets.json") as secrets_file:
    secrets_json = secrets_file.read()
    secrets = load_json( secrets_json )

    secrets_file.close()


TOKEN: Final[str] = secrets["token"]

CHANNEL_RULES: Final[int] = 1300928626392301750
CHANNEL_TRASH: Final[int] = 1302776301161287752

BOT_PANCAKE: Final[int] = 239631525350604801



# bot setup
intents: Intents = Intents.default()
intents.message_content = True

client: Client = Client(intents=intents)


def bad_logging(message: Message) -> None:
    username: str = str(message.author)
    channel: str = str(message.channel)
    print(f"[{channel}] {username}: \"{message.content}\"")
    return


def in_rules_channel(channel_id: int) -> bool:
    return (channel_id == CHANNEL_RULES)


async def get_last_rule_num(message: Message) -> int:
    channel = client.get_channel( CHANNEL_RULES )

    messages = []

    async for msg in channel.history(limit=2):
        messages.append(msg)
        #print(msg.content)

    #for i in range( len(messages) ):
    #    print(f"current msg at {i}: {messages[i].content}")

    last_message = str( messages[1].content )
    #print("last message: ", messages[0].content)
    #print("last message: ", last_message)
    
    # get the rule number here 
    formatted = last_message[6:]
    index = formatted.find(':')
    formatted = formatted[:index]

    return int(formatted)


async def formatted(message: Message) -> bool:
    lowered: str = message.content.lower()

    index: int = lowered.find(':')
    if (index < 0):
        return False

    rule_num: int = await get_last_rule_num(message) + 1
    #print( rule_num )
    #print( lowered[:index+2] )
    #print( f"rule #{rule_num}: " )

    return (lowered[:index+2] == f"rule #{rule_num}: ")


# message functionality
async def check_rule(message: Message) -> None:
    #bad_logging(message)

    if not message.content:
        print("Message probably because intents were not enabled")
        return



    if not ( await formatted(message) ):
        await message.delete()

    return


async def move_output(message: Message) -> None:
    message.content = "Moved pancake message: \n" + message.content
    
    trash = client.get_channel(CHANNEL_TRASH)
    if (len(message.embeds) > 0):
        await trash.send(message.content, embed=message.embeds[0])
    else:
        trash.send(message.content)

    await message.delete()
    return


@client.event
async def on_ready() -> None:
    print(f"{client.user} is now running!")
    return


@client.event 
async def on_message(message: Message) -> None:
    # if message is from our bot, ignore
    if (message.author == client.user):
        return

    bad_logging(message)

    if (message.author.id == BOT_PANCAKE):
        await move_output(message)
    
    if in_rules_channel( message.channel.id ):
        await check_rule(message)

    return


def main() -> None:
    client.run(token=TOKEN)
    return


if (__name__ == "__main__"):
    main()