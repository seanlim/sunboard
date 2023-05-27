import pcbnew

pcb = pcbnew.GetBoard()


def get_footprints():
    return [f.GetReference() for f in pcb.Footprints()]

def get_footprint(name):
    return [f for f in pcb.Footprints() if f.GetReference() == name][0]

def place_component_relative(reference_component_name, move_component_name, relative_x, relative_y):
    ref = get_footprint(reference_component_name)
    move = get_footprint(move_component_name)

    move.SetPosition(ref.GetPosition() + pcbnew.VECTOR2I_MM(relative_x, relative_y))

for y in range(0, 14):
    for x in range(1, 11):
        place_component_relative("D" + str(x + y * 11), "D" + str(x+1 + y * 11), 10, 0)
    place_component_relative("D" + str(y * 11 + 1), "D" + str((y+1) * 11 + 1), 0, 15)

for y in range(0, 14):
    for x in range(1, 11):
        place_component_relative("SW" + str(x + y * 11), "SW" + str(x+1 + y * 11), 10, 0)
    place_component_relative("SW" + str(y * 11 + 1), "SW" + str((y+1) * 11 + 1), 0, 15)
pcbnew.Refresh()
