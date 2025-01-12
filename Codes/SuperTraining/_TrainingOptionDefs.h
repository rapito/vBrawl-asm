#ifndef SUPERTRAINING_TRAININGOPTIONDEFS_H
#define SUPERTRAINING_TRAININGOPTIONDEFS_H

#define FUDGE_DEBUG_TOOLS 0
// helps debug PSA by showing FSM and the individual AnimCmd values
#define PSA_DEBUGGER_PAGE 1
// shows various stats about PSA/attacks n such
#define PSA_DATA_PAGE 1

// shows positional data and momentum-related stats
#define POSITIONAL_DATA_PAGE 1

// shows a line that displays the traectory of a fighter
// maybe useful for newcomers
#define TRAJECTORY_LINE_PAGE 0

// displays a heatmap of where hitboxes go
#define HITBOX_HEATMAP_PAGE 1
// used to help figure out the range of projectiles, attacks, etc
#define HITBOX_HEATMAP_BOUNDS 0

// used to help aid in AI development and/or maybe help
// break down what an AI is thinking to an onlooker
#define AI_DEBUGGING_PAGE 1
// used to customize and view AI behavior
#define AI_STAT_PAGES 1
// good for training
#define COMBO_TRAINER_PAGE 1
// good for tournament settings
#define CONTROLLER_INFO_PAGE 0

#endif