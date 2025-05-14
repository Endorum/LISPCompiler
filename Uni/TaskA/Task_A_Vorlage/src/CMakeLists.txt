# Model
set(ModelSources
    ${CMAKE_CURRENT_SOURCE_DIR}/model/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/model/Platform.cpp
    )

# Controller
set(ControlSources
    ${CMAKE_CURRENT_SOURCE_DIR}/control/PlayerControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/Game.cpp
    )

# View
set(ViewSources
    ${CMAKE_CURRENT_SOURCE_DIR}/view/Layer.cpp
    )

# add src code to target
add_executable(Platformer
        ${ModelSources}
        ${ControlSources}
        ${ViewSources}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )