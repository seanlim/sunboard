import { Move } from "./routes";

function isOdd(n: number) {
  return n % 2 == 1;
}

export function convertMovesToString(moves: Move[]): string {
  const holds: string[] = [];
  moves.forEach((move) => {
    // Column
    const colCharCode = move.description.toUpperCase().charCodeAt(0);
    const colIndex = colCharCode - 65;
    // rowNumber is bottom-up
    const rowNumber = Number.parseInt(move.description.substring(1));
    const holdNumber =
      colIndex * 18 +
      (isOdd(colIndex)
        ? 19 - rowNumber // From top
        : rowNumber) -
      1;
    if (move.isStart) {
      holds.push(`S${holdNumber}`);
    } else if (move.isEnd) {
      holds.push(`E${holdNumber}`);
    } else {
      holds.push(`P${holdNumber}`);
    }
  });
  return `l#${holds.join(",")}#`;
}

export function stringToByteArray(str: string): number[] {
  let byteArray = new Uint8Array(str.length);
  for (let i = 0; i < str.length; i++) {
    byteArray[i] = str.charCodeAt(i) & 0xff;
  }
  return Array.from(byteArray);
}
