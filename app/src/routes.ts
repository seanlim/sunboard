const mb2016Routes = require("./problems MoonBoard 2016 .json");

type Move = {
  description: string;
  isStart: boolean;
  isEnd: boolean;
};

export type MBRoute = {
  name: string;
  moves: Move[]; // TODO: parse this into correct format
  grade: string;
  setby: string;
};

/* Example Route
{
    "name": "Far from the Madding Crowd",
    "grade": "6B+",
    "userGrade": "6B+",
    "setbyId": "5FC09F63-05F3-4DAE-A1A5-3AC22C37139A",
    "setby": "Ben Moon",
    "method": "Feet follow hands",
    "userRating": 4,
    "repeats": 26546,
    "holdsetup": {
        "description": "MoonBoard 2016",
        "holdsets": null,
        "apiId": 1
    },
    "isBenchmark": true,
    "isMaster": false,
    "upgraded": false,
    "downgraded": false,
    "moves": [
        {
            "problemId": 19215,
            "description": "E6",
            "isStart": true,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "C5",
            "isStart": true,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "E8",
            "isStart": false,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "F11",
            "isStart": false,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "C13",
            "isStart": false,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "D15",
            "isStart": false,
            "isEnd": false
        },
        {
            "problemId": 19215,
            "description": "D18",
            "isStart": false,
            "isEnd": true
        }
    ],
    "holdsets": [
        {
            "description": "Hold Set B",
            "locations": null,
            "apiId": 5
        }
    ],
    "hasBetaVideo": true,
    "moonBoardConfigurationId": 0,
    "apiId": 19215,
    "dateInserted": "2016-03-01T10:00:27",
    "dateUpdated": "2023-05-04T02:27:01.853",
    "dateDeleted": null
}
*/

export function get2016Routes(): MBRoute[] {
  // TODO: parse moves array into string representation
  return mb2016Routes["data"];
}
