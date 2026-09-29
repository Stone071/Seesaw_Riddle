// The possible islanders
const islanderNames: string[] = [" ", "Ana", "Bruno", "Celia", "Doris", "Evinrude", "Franklin", "Gurt", "Harry", "Ingrid", "Jazeera", "Konklin", "Leonard"];

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
// Oops. Need to make sure you can't place the same player twice.
// I should make a struct for the islanders rather than just names
for (let i:number = 0; i < 12; i++)
{
    if (posButtons[i]) {
        posButtons[i].addEventListener("click", () => {
            for (let j:number = 0; j < 13; j++)
            {
                if (posButtons[i].textContent == islanderNames[j])
                {
                    const next:number = (j < 12) ? (j+1) : 0;
                    posButtons[i].textContent = islanderNames[next];
                    break;
                }
            }
        });
    }
}

// Attach event listeners for the control buttons
