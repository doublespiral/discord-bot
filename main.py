from json import loads as load_json
from typing import Final

from discord import Intents, Client, Message



with open("secrets.json") as secrets_file:
    secrets_json = secrets_file.read()
    secrets = load_json( secrets_json )

    secrets_file.close()
    #print(secrets["token"])


TOKEN: Final[str] = secrets["token"]
ENFORCED_CHANNEL_ID: Final[int] = 1300928626392301750


# bot setup
intents: Intents = Intents.default()
intents.message_content = True

client: Client = Client(intents=intents)



def enforced_channel(channel_id: int) -> bool:
    return ( channel_id == ENFORCED_CHANNEL_ID)


async def get_last_rule_num(message: Message) -> int:
    channel = client.get_channel( ENFORCED_CHANNEL_ID )

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
async def validate_message(message: Message) -> None:
    if not message.content:
        print("Message probably because intents were not enabled")
        return

    if not enforced_channel( message.channel.id ):
        return

    if not ( await formatted(message) ):
        await message.delete()
    
    return


@client.event
async def on_ready() -> None:
    print(f"{client.user} is now running!")
    return


@client.event 
async def on_message(message: Message) -> None:
    
    # if message is from bot, ignore
    if (message.author == client.user):
        return 
    
    username: str = str(message.author)
    channel: str = str(message.channel)

    print(f"[{channel}] {username}: \"{message.content}\"")

    await validate_message(message)

    return


def main() -> None:
    client.run(token=TOKEN)
    return


if (__name__ == "__main__"):
    main()