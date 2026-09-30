// For each islander, we need a name and current position
type Islander = {
    readonly name: string;
    position: number;
};

// The possible islanders. Islander 12 " " is an empty spot on the seesaw.
// Position 12 means not on seesaw.
const islanderList: Islander[] = [
    {name:"Ana", position:12}, 
    {name:"Bruno", position:12},
    {name:"Celia", position:12}, 
    {name:"Doris", position:12}, 
    {name:"Evinrude", position:12}, 
    {name:"Franklin", position:12}, 
    {name:"Gurt", position:12}, 
    {name:"Harry", position:12}, 
    {name:"Ingrid", position:12}, 
    {name:"Jazeera", position:12}, 
    {name:"Konklin", position:12}, 
    {name:"Leonard", position:12},
    {name:" ", position:12},
];

// Grab DOM Elements
// Control buttons
const testButton = document.getElementById("test-btn") as HTMLButtonElement;
const discIslanderButton = document.getElementById("get-islander-btn") as HTMLButtonElement;
const discWeightButton = document.getElementById("get-weight-btn") as HTMLButtonElement;
// Create an array of all the positional buttons
const posButtons: HTMLButtonElement[] = [];
for (let i:number = 0; i < 12; i++)
{
    const name:string = "pos-" + i.toString();
    posButtons[i] = document.getElementById(name) as HTMLButtonElement;
}

// Give each position button an event listener for changing names
for (let buttonNum:number = 0; buttonNum < 12; buttonNum++)
{
    if (posButtons[buttonNum]) 
    {
        posButtons[buttonNum].addEventListener("click", () => {
            // 1. Remember the current occupant
            const currentName:string = posButtons[buttonNum].textContent;
            let currentIslander:number = GetIslanderIndexFromName(currentName);
            // Remove currentIslander
            if (currentIslander >= 0 && currentIslander < islanderList.length) {
                islanderList[currentIslander].position = 12;
                posButtons[buttonNum].textContent = " ";
            }

            // 2. Place the NEXT occupant
            for (let offset:number = 1; offset < islanderList.length; offset++) {
                // Wrap around after index 12
                const index:number = (currentIslander + offset) % islanderList.length;
                if(islanderList[index].position == 12) {
                    if (islanderList[index].name != " ") {
                        // Leave " " islander with position 12 so they can be placed again
                        islanderList[index].position = buttonNum;
                    }
                    posButtons[buttonNum].textContent = islanderList[index].name;
                    break;
                }
            }
            
            //DebugDump();
        });
    }
}

// Take input of name and return index in islanderList
function GetIslanderIndexFromName(islanderName:string): number {
    // If we found nobody, return -1.
    let retVal:number = -1;
    for (let i:number = 0; i < islanderList.length; i++){
        if (islanderList[i].name == islanderName)
        {
            retVal = i;
        }
    }
    return retVal;
}

// Dump the islanders to debug frame
function DebugDump() {
    let bigStr:string = ""
    islanderList.forEach(element => {
        bigStr += element.name + "," + element.position.toString() + ",\n";
    });
    if (document != null)
    {
        document.getElementById("debug-frame").innerText = bigStr;
    }
}

// Attach event listeners for the control buttons
