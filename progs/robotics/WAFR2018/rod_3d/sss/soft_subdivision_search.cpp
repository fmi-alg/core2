/*
 * soft_subdivision_search.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "sss/soft_subdivision_search.h"

extern MainWindow* window;

void printConfBoxes() {
  std::string status_names[4] = {"FREE", "STUCK", "MIXED", "UNKNOWN"};
  for (const auto& box : ConfBox3d::boxes) {
    std::cout << "===================" << std::endl;
    std::cout << "Box " << box->box_id << ":" << std::endl;
    std::cout << box->center.X() << "\t" << box->center.Y() << "\t"
              << box->center.Z() << "\t" << box->width << "\t"
              << status_names[box->status] << std::endl;
    for (const auto& neighbor : box->neighbors) {
      std::cout << neighbor->box_id << "\t";
    }
    std::cout << std::endl;
    std::cout << "===================" << std::endl;
  }
}

// TODO: Let the constructor accept start and goal as configurations of the
// robot
SoftSubdivisionSearch::SoftSubdivisionSearch(ConfBox3d* root, double epsilon,
                                             int search_type, Point3d start,
                                             Point3d goal,
                                             Point3d start_rotation,
                                             Point3d goal_rotation)
    : epsilon_(epsilon),
      search_type_(search_type),
      ct_(0),
      root(root),
      free_count(0),
      stuck_count(0),
      mix_count(0) {
  this->start_ = start;
  this->goal_ = goal;
  this->start_rotation_ = start_rotation;
  this->goal_rotation_ = goal_rotation;

  switch (search_type_) {
    case RANDOM:
      PQ_ = new RandQueue();
      break;
    case BFS:
      PQ_ = new SeqQueue();
      break;
    case GREEDY:
      PQ_ = new DijkstraQueue<DistCmp>();
      break;
    case BIGREEDY:
      PQ_ = new DijkstraQueue<BiDistCmp>();
      break;
    case GREEDY_SIZE:
      PQ_ = new DijkstraQueue<DistSizeCmp>();
      break;
    case BIGREEDY_SIZE:
      PQ_ = new DijkstraQueue<BiDistSizeCmp>();
      break;
    case VORONOI:
      PQ_ = new DijkstraQueue<VorCmp>();
      break;
    case BIVORONOI:
      PQ_ = new DijkstraQueue<BiVorCmp>();
      break;
    default:
      std::cerr << "Wrong search_type" << std::endl;
      exit(1);
  }

  predicate_ = new ConfBox3dPredicate();
  if ((search_type_ == VORONOI || search_type_ == BIVORONOI) &&
      (!root->vor_corners.empty() || !root->vor_edges.empty() ||
       !root->vor_walls.empty())) {
    predicate_->findVorCleanrance(root, root->rod);
  }

  if (!root->corners.empty() || !root->edges.empty() || !root->walls.empty()) {
    root->status = MIXED;
  }
  insertNode(root);
}

void SoftSubdivisionSearch::insertNode(ConfBox3d* box) {
  PQ_->process(box);

  switch (box->getStatus()) {
    case FREE:
      unionAdjacent(box);
      ++free_count;
      break;
    case STUCK:
      ++stuck_count;
      break;
    case MIXED:
      ++mix_count;
      if (box->width < epsilon_) {
      }
      break;
    case UNKNOWN:
      std::cout << "UNKNOWN not handled" << std::endl;
      break;
    default:
      std::cerr << "Wrong Status" << std::endl;
      exit(1);
  }
}

bool SoftSubdivisionSearch::expand(ConfBox3d* box) {
  if (box->split(epsilon_)) {
    for (const auto& child : box->children) {
      if (search_type_ == VORONOI || search_type_ == BIVORONOI) {
        predicate_->checkVoronoiFeatureSet(child, box);
        predicate_->findVorCleanrance(child, child->rod);
      }
      predicate_->checkCollisionFeatureSet(child, box);
      insertNode(child);
    }
    return true;
  }
  return false;
}

void SoftSubdivisionSearch::unionAdjacent(ConfBox3d* box) {
  if (box->getStatus() != FREE) {
    std::cerr << "Cannot union boxes that are mixed or stuck" << std::endl;
    exit(1);
  }
  if (box->p_set == 0) {
    new Set(box);
  }
  for (const auto& neighbor : box->neighbors) {
    if (neighbor->status == FREE) {
      if (neighbor->p_set == 0) {
        new Set(neighbor);
      }
      p_sets_->Union(box, neighbor);
    }
  }
}

ConfBox3d* SoftSubdivisionSearch::findEnclosingFreeBox(Point3d point,
                                                       Point3d point_rotation) {
  ConfBox3d* box = root->getBox(point, point_rotation);
  while (box != nullptr && !(box)->is_free()) {
    if (!expand(box)) {
      return 0;  // Does not have a free box for the given resolution
    }
    ++ct_;
    box = box->getBox(point, point_rotation);
  }
  return box;
}

template <typename Cmp>
bool SoftSubdivisionSearch::findPath(ConfBox3d* boxA, ConfBox3d* boxB) {
  boxA->is_in_source = true;
  DijkstraQueue<Cmp> dijQ;
  dijQ.push(boxA);

  DijkstraQueue<Cmp> dijQ2;
  if (search_type_ == BIGREEDY || search_type_ == BIGREEDY_SIZE ||
      search_type_ == BIVORONOI) {
    boxB->is_in_source = true;
    dijQ2.push(boxB);
  }

  bool alter = true;

  while (!p_sets_->isConnect(boxA, boxB) && !dijQ.empty()) {
    ConfBox3d* current;

    if (search_type_ == BIGREEDY || search_type_ == BIGREEDY_SIZE ||
        search_type_ == BIVORONOI) {
      if (dijQ.empty() || dijQ2.empty()) {
        break;
      }
      if (alter)
        current = dijQ.extract();
      else
        current = dijQ2.extract();
    } else {
      current = dijQ.extract();
    }

    // if current is MIXED, try expand it and push the children that is
    // ACTUALLY neighbors of the source set (set containing start) into the dijQ
    // again
    if (current->status == MIXED) {
      if (expand(current)) {
        ++ct_;
        for (auto&& child : current->children) {
          if (child->in_queue) continue;

          // go through neighbors of each child to see if it's in source set
          // if yes, this child go into the dijQ
          bool is_in_source = false;
          for (const auto& neighbor : child->neighbors) {
            if (neighbor->is_in_source) {
              is_in_source = true;
              break;
            }
          }

          if (is_in_source) {
            switch (child->getStatus()) {
              // if it's FREE, also insert to source set
              case FREE:
              case MIXED:
                child->in_queue = true;
                child->bidirection = current->bidirection;

                if (alter)
                  dijQ.push(child);
                else
                  dijQ2.push(child);
                break;
              case STUCK:
                // cerr << "inside FindPath: STUCK case not treated" << endl;
                break;
              case UNKNOWN:
                // cerr << "inside FindPath: UNKNOWN case not treated" << endl;
                break;
            }
          }
        }
      }  // end if (expand(current))
    }    // end if (current->status == MIXED)

    // found path!
    if (current == boxB &&
        !(search_type_ == BIGREEDY || search_type_ == BIGREEDY_SIZE ||
          search_type_ == BIVORONOI)) {
      return true;
    }

    if (current->status == FREE) {
      current->is_in_source = true;
      // if current is not MIXED, then must be FREE
      // go through it's neighbors and add FREE and MIXED ones to dijQ
      // also add FREE ones to source set
      for (auto&& neighbor : current->neighbors) {
        if (!neighbor->in_queue && !neighbor->is_in_source) {
          if (neighbor->status == FREE || neighbor->status == MIXED) {
            neighbor->in_queue = true;
            neighbor->bidirection = current->bidirection;
            if (neighbor->status == FREE) neighbor->is_in_source = true;
            if (alter)
              dijQ.push(neighbor);
            else
              dijQ2.push(neighbor);
          }
        }
      }
    }  // end if (current->status == FREE)

    if (search_type_ == BIGREEDY || search_type_ == BIGREEDY_SIZE ||
        search_type_ == BIVORONOI)
      alter = !alter;
  }

  if (!(search_type_ == BIGREEDY || search_type_ == BIGREEDY_SIZE ||
        search_type_ == BIVORONOI))
    return !dijQ.empty();
  else
    return !dijQ.empty() && !dijQ2.empty();
}

// based on findPath concept to find the voronoi box we want
// the strategy of the comparison function is basically
// to increase the clearance (move away from the closest feature)
template <typename Cmp>
ConfBox3d* SoftSubdivisionSearch::findVorBox(ConfBox3d* free_box) {
  free_box->is_in_source = true;
  DijkstraQueue<Cmp> dijQ;
  dijQ.push(free_box);

  while (!dijQ.empty()) {
    ConfBox3d* current = dijQ.extract();

    // if current is MIXED, try expand it and push the children that is
    // ACTUALLY neighbors of the source set (set containing start) into the dijQ
    if (current->status == MIXED) {
      if (expand(current)) {
        ++ct_;
        for (auto&& child : current->children) {
          // go through neighbors of each child to see if it's in source set
          // if yes, this child go into the dijQ
          bool is_in_source = false;
          for (const auto& neighbor : child->neighbors) {
            if (neighbor->is_in_source) {
              is_in_source = true;
              break;
            }
          }

          if (is_in_source) {
            switch (child->getStatus()) {
              // if it's FREE, also insert to source set
              case FREE:
                child->is_in_source = true;
              case MIXED:
                child->in_vor_queue = true;
                dijQ.push(child);
                break;
              case STUCK:
                // cerr << "inside FindPath: STUCK case not treated" << endl;
                break;
              case UNKNOWN:
                // cerr << "inside FindPath: UNKNOWN case not treated" << endl;
                break;
            }
          }
        }  // end for loop current's children
      }    // end if(expand(current))
    }      // end if (current->status == MIXED)

    // found path!
    if (current->status == FREE && current->vor_corners.size() +
                                           current->vor_edges.size() +
                                           current->vor_walls.size() >
                                       2) {
      return current;
    }

    if (current->status == FREE) {
      current->is_in_source = true;
      // current is FREE
      // go through it's neighbors and add FREE and MIXED ones to dijQ
      // also add FREE ones to source set!!!
      for (auto&& neighbor : current->neighbors) {
        if (!neighbor->in_vor_queue && !neighbor->is_in_source &&
            (neighbor->status == FREE || neighbor->status == MIXED)) {
          neighbor->in_vor_queue = true;
          if (neighbor->status == FREE) neighbor->is_in_source = true;
          dijQ.push(neighbor);
        }
      }
    }  // end if (current->status == FREE)
  }    // end while(!dijQ.empty())

  return free_box;
}

// The main SSS Framework as described in the RSS 2013 RCV Paper, section 7
std::vector<ConfBox3d*> SoftSubdivisionSearch::softSubdivisionSearch() {
  ConfBox3d::boxes.clear();
  std::vector<ConfBox3d*> path;

  // 1. Initialization
  move_away = true;  // flag for finding voronoi box
  ConfBox3d* boxA = findEnclosingFreeBox(start, start_rotation);
  if (boxA == nullptr) {
    (*window) << "STUCK: Initial configuration is not free\n";
    return path;
  }
  boxA->bidirection = 1;
  if (search_type_ == VORONOI || search_type_ == BIVORONOI) {
    boxA = findVorBox<DistCmpSpecial>(boxA);
    if (boxA == nullptr) {
      (*window) << "STUCK: Cannot find Vor box A\n";
      return path;
    }
  }
  boxA->bidirection = 1;

  ConfBox3d* boxB = findEnclosingFreeBox(goal, goal_rotation);
  if (boxB == nullptr) {
    (*window) << "STUCK: Final configuration is not free\n";
    return path;
  }
  boxB->bidirection = -1;
  if (search_type_ == VORONOI || search_type_ == BIVORONOI) {
    boxB = findVorBox<DistCmpSpecial>(boxB);
    if (boxB == nullptr) {
      (*window) << "STUCK: Cannot find Vor box B\n";
      return path;
    }
  }
  boxB->bidirection = -1;

  move_away = false;
  // 2. Main
  if (search_type_ == RANDOM || search_type_ == BFS) {
    while (!p_sets_->isConnect(boxA, boxB)) {
      if (PQ_->empty()) {
        path.clear();
        return path;
      }
      ConfBox3d* box = PQ_->extract();
      // b might not be a leaf since it could already be split in
      // expand(ConfBox3d* b), and PQ is not updated there
      if (box->isLeaf() && box->getStatus() == MIXED) {
        expand(box);
      }
      ++ct_;
    }
  } else if (search_type_ == GREEDY) {
    bool no_path = !findPath<DistCmp>(boxA, boxB);
    if (no_path) return path;
  } else if (search_type_ == BIGREEDY) {
    bool no_path = !findPath<BiDistCmp>(boxA, boxB);
    if (no_path) return path;
  } else if (search_type_ == GREEDY_SIZE) {
    bool no_path = !findPath<DistSizeCmp>(boxA, boxB);
    if (no_path) return path;
  } else if (search_type_ == BIGREEDY_SIZE) {
    bool no_path = !findPath<BiDistSizeCmp>(boxA, boxB);
    if (no_path) return path;
  } else if (search_type_ == VORONOI) {
    bool no_path = !findPath<VorCmp>(boxA, boxB);
    if (no_path) return path;
  } else if (search_type_ == BIVORONOI) {
    bool no_path = !findPath<BiVorCmp>(boxA, boxB);
    if (no_path) return path;
  }

  // 3. Compute Free Channel
  path = Path::dijkstraShortestPath(boxA, boxB);
  path.insert(path.begin(), ConfBox3d::makePointBox(goal, goal_rotation));
  path.push_back(ConfBox3d::makePointBox(start, start_rotation));

  return path;
}
