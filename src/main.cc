#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "key_data.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

constexpr bool kWriteTestKeys = false;
constexpr bool kIncludeDactylRef = false;
// Add the caps into the stl for testing.
constexpr bool kAddCaps = false;

enum class Direction { UP, DOWN, LEFT, RIGHT };

void AddShapes(std::vector<Shape>* shapes, std::vector<Shape> to_add) {
  for (Shape s : to_add) {
    shapes->push_back(s);
  }
}

Shape ConnectMainKeys(KeyData& d);

int main() {
  printf("generating..\n");
  TransformList key_origin;
  //  key_origin.RotateY(10);
  key_origin.Translate(-20, -40, 3);

  // This is where all of the logic to position the keys is done. Everything below is cosmetic
  // trying to build the case.
  KeyData d(key_origin);

  if (kWriteTestKeys) {
    std::vector<Shape> test_shapes;
    std::vector<Key*> test_keys;
     test_keys = d.all_keys();
    for (Key* key : test_keys) {
      key->add_side_nub = false;
      key->add_top_nub = false;
      key->extra_z = 2;
      test_shapes.push_back(key->GetSwitch());
      if (kAddCaps) {
        test_shapes.push_back(key->GetCap().Color("red"));
      }
    }
    if (kIncludeDactylRef) {
      Shape dactyl_manuform = Import("dactyl_manuform.stl").Translate(9, 8, -3.5);
      Shape dactyl_cc = Import("dactyl_cc.stl");
      test_shapes.push_back(dactyl_manuform.Color("green", .6));
      //  test_shapes.push_back(dactyl_cc.Color("blue", .3));
    }
    UnionAll(test_shapes)
        .WriteToFile("test_keys.scad");
    return 0;
  }

  // Set all of the widths here. This must be done before calling any of GetTopLeft etc.


d.key_th1.extra_width_top = 2;
  d.key_th1.extra_width_left = 2;

  d.key_th2.extra_width_top = 2;
  d.key_th3.extra_width_right = 2;
  d.key_th_top3.extra_width_right = 1;
  d.key_th_top3.extra_width_top = 2;

  d.key_v.extra_width_bottom = 4;

  d.key_m2.extra_width_right = 3;

  d.key_plus.extra_width_top = 0;
  d.key_1.extra_width_top = 4.5;
  d.key_2.extra_width_top = 0;
  d.key_4.extra_width_top = 1;
  d.key_5.extra_width_top = 2.5;

  d.key_plus.extra_width_left = 2;
  d.key_tab.extra_width_left = 2;
  d.key_caps.extra_width_left = 2;
  d.key_shift.extra_width_left = 2;
  d.key_fn.extra_width_left = 2;

  
  d.key_m1.extra_width_top = 1.25;
  d.key_m1.extra_width_right = 2;
  d.key_m2.extra_width_right = 2;
  d.key_m3.extra_width_right = 2;

  d.key_fn.extra_width_bottom = 4.75;
  d.key_tilde.extra_width_bottom = 0;
  d.key_slash.extra_width_bottom = 7;


  

  std::vector<Shape> shapes;

  //
  // Thumb plate
  //

  shapes.push_back(ConnectMainKeys(d));
  shapes.push_back(Union(ConnectVertical(d.key_q, d.key_a),
                         ConnectVertical(d.key_a, d.key_z),
                         ConnectHorizontal(d.key_th2, d.key_th3),
                         ConnectHorizontal(d.key_th1, d.key_th2)));

  shapes.push_back(TriMesh({
      d.key_th1.GetBottomLeft(),
      d.key_slash.GetBottomRight(),
      d.key_left_arrow.GetBottomRight(),
      d.key_left_arrow.GetBottomLeft(),
  }));
  
  shapes.push_back(TriMesh({
      d.key_th1.GetBottomLeft(),
      d.key_th1.GetTopLeft(),
      d.key_left_arrow.GetBottomRight(),
  }));

  shapes.push_back(TriFan(d.key_v.GetBottomLeft(),
                          {
                              d.key_v.GetBottomRight(),
                              d.key_th1.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_v.GetBottomRight(),
                          {
                              d.key_b.GetBottomLeft(),
                              d.key_th1.GetTopRight(),
                              d.key_th1.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_v.GetBottomLeft(),
                          {
                              d.key_c.GetBottomRight(),
                              d.key_left_arrow.GetTopRight(),
                              d.key_left_arrow.GetBottomRight(),
                              d.key_th1.GetTopLeft(),
                          }));

  shapes.push_back(TriFan(d.key_th_top3.GetBottomLeft().Apply(GetPostConnector()),
                          {
                              d.key_th3.GetTopLeft().Apply(GetPostConnector()),
                              d.key_th3.GetTopRight().Apply(GetPostConnector()),
                              d.key_th_top3.GetBottomRight().Apply(GetPostConnector()),
                          }));

  shapes.push_back(TriMesh({
      d.key_th_top3.GetTopRight(),
      d.key_th_top3.GetTopLeft(),
      d.key_g.GetBottomRight(),
  }));
  shapes.push_back(TriMesh({
      d.key_th_top3.GetTopLeft(-1).Apply(GetPostConnector(2)),
      d.key_th_top3.GetBottomLeft(-1).Apply(GetPostConnector(2)),
      d.key_th2.GetTopRight().Apply(GetPostConnector()),
      d.key_th3.GetTopLeft().Apply(GetPostConnector()),
  }));
  shapes.push_back(TriFan(d.key_b.GetBottomRight(),
                          {
                              d.key_b.GetTopRight(),
                              d.key_g.GetBottomRight(),
                              d.key_th_top3.GetTopLeft(),
                              d.key_th2.GetTopRight(),
                              d.key_th2.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_th2.GetTopLeft(),
                          {
                              d.key_th1.GetTopRight(),
                              d.key_b.GetBottomLeft(),
                              d.key_b.GetBottomRight(),
                          }));

shapes.push_back(TriFan(d.key_m3.GetBottomLeft(),
                          {
                              d.key_th_top3.GetTopRight(),
                              d.key_th_top3.GetTopLeft(),
                              d.key_m3.GetBottomRight(),
                          }));

shapes.push_back(TriFan(d.key_m3.GetBottomRight(),
                          {
                              d.key_th_top3.GetTopRight(),
                              d.key_th_top3.GetTopLeft(),
                              d.key_m3.GetBottomRight(),
                          }));

  shapes.push_back(TriFan(d.key_th_bottom2.GetTopRight(),
                          {
                              d.key_th_bottom2.GetBottomRight(),
                              d.key_th3.GetBottomRight(),
                              d.key_th3.GetBottomLeft(),
                          }));
  shapes.push_back(TriFan(d.key_th_bottom2.GetTopLeft(),
                          {
                              d.key_th3.GetBottomLeft(),
                              d.key_th2.GetBottomRight(),
                              d.key_th2.GetBottomLeft(),
                          }));
  shapes.push_back(TriFan(d.key_th_bottom2.GetBottomLeft(),
                          {
                              d.key_th1.GetBottomLeft(),
                              d.key_th1.GetBottomRight(),
                          }));
  shapes.push_back(TriFan(d.key_th1.GetBottomRight(),
                          {
                              d.key_th_bottom2.GetBottomLeft(),
                              d.key_th_bottom2.GetTopLeft(),
                              d.key_th2.GetBottomLeft(),
                          }));
  shapes.push_back(Tri(d.key_th_bottom2.GetTopLeft().Apply(GetPostConnector()),
                       d.key_th_bottom2.GetTopRight().Apply(GetPostConnector()),
                       d.key_th3.GetBottomLeft(-1).Apply(GetPostConnector(2))));

  //
  // Make the wall
  //
  {
    struct WallPoint {
      WallPoint(TransformList transforms,
                Direction out_direction,
                float extra_distance = 0,
                float extra_width = 0)
          : transforms(transforms),
            out_direction(out_direction),
            extra_distance(extra_distance),
            extra_width(extra_width) {
      }
      TransformList transforms;
      Direction out_direction;
      float extra_distance;
      float extra_width;
    };

    Direction up = Direction::UP;
    Direction down = Direction::DOWN;
    Direction left = Direction::LEFT;
    Direction right = Direction::RIGHT;

    std::vector<WallPoint> wall_points = {
       {d.key_plus.GetTopLeft(), up},
       {d.key_plus.GetTopRight(), up},
 
        {d.key_1.GetTopLeft(), up},
        {d.key_1.GetTopRight(), up},
        {d.key_2.GetTopLeft(), up},
        {d.key_2.GetTopRight(), up},
        {d.key_3.GetTopLeft(), up},
        {d.key_3.GetTopRight(), up},
        {d.key_4.GetTopLeft(), up},
        {d.key_4.GetTopRight(), up},
        {d.key_5.GetTopLeft(), up},
        {d.key_5.GetTopRight(), up},
        {d.key_m1.GetTopLeft(), up},
        {d.key_m1.GetTopRight(), up},

        //{d.key_4.GetTopRight(), up, 1.5},
        {d.key_m1.GetTopRight(), right},
        {d.key_m1.GetBottomRight(), right},
        {d.key_m2.GetTopRight(), right},
        {d.key_m2.GetBottomRight(), right},
        {d.key_m3.GetTopRight(), right},
        {d.key_m3.GetBottomRight(), right, 1, .5},

        //{d.key_th_top3.GetTopRight(), up, 1, 1},
        {d.key_th_top3.GetTopRight(), right},
        {d.key_th_top3.GetBottomRight(), right},

        {d.key_th3.GetTopRight(), right},
        {d.key_th3.GetBottomRight(), right},

        {d.key_th_bottom2.GetBottomRight(), right},
        {d.key_th_bottom2.GetBottomRight(), down},
        {d.key_th_bottom2.GetBottomLeft(), down},
        {d.key_th1.GetBottomLeft(), down},

        {d.key_slash.GetBottomRight(), down},
        {d.key_slash.GetBottomLeft(), down},
        {d.key_tilde.GetBottomRight(), down},
        {d.key_tilde.GetBottomLeft(), down},
        {d.key_fn.GetBottomRight(), down},
        {d.key_fn.GetBottomLeft(), down},

        //{d.key_fn.GetBottomLeft(), down, 0, .75},
        {d.key_fn.GetBottomLeft(), left, 0, .5},
        {d.key_fn.GetTopLeft(), left, 0, .5},

        {d.key_caps.GetBottomLeft(), left},
        {d.key_caps.GetTopLeft(), left},

        {d.key_tab.GetBottomLeft(), left},
        {d.key_tab.GetTopLeft(), left},

        {d.key_plus.GetBottomLeft(), left},
        {d.key_plus.GetTopLeft(), left},

    };

    std::vector<std::vector<Shape>> wall_slices;
    for (WallPoint point : wall_points) {
      Shape s1 = point.transforms.Apply(GetPostConnector());

      TransformList t = point.transforms;
      glm::vec3 out_dir;
      float distance = 4.8 + point.extra_distance;
      switch (point.out_direction) {
        case Direction::UP:
          t.AppendFront(TransformList().Translate(0, distance, 0).RotateX(-20));
          break;
        case Direction::DOWN:
          t.AppendFront(TransformList().Translate(0, -1 * distance, 0).RotateX(20));
          break;
        case Direction::LEFT:
          t.AppendFront(TransformList().Translate(-1 * distance, 0, 0).RotateY(-20));
          break;
        case Direction::RIGHT:
          t.AppendFront(TransformList().Translate(distance, 0, 0).RotateY(20));
          break;
      }

      // Make sure the section extruded to the bottom is thick enough. With certain angles the
      // projection is very small if you just use the post connector from the transform. Compute
      // an explicit shape.
      const glm::vec3 post_offset(0, 0, -4);
      const glm::vec3 p = point.transforms.Apply(post_offset);
      const glm::vec3 p2 = t.Apply(post_offset);

      glm::vec3 out_v = p2 - p;
      out_v.z = 0;
      const glm::vec3 in_v = -1.f * glm::normalize(out_v);

      float width = 3.3 + point.extra_width;
      Shape s2 = Hull(Cube(.1).Translate(p2), Cube(.1).Translate(p2 + (width * in_v)));

      std::vector<Shape> slice;
      slice.push_back(Hull(s1, s2));
      slice.push_back(Hull(s2, s2.Projection().LinearExtrude(.1).TranslateZ(.05)));

      wall_slices.push_back(slice);
    }

    for (size_t i = 0; i < wall_slices.size(); ++i) {
      auto& slice = wall_slices[i];
      auto& next_slice = wall_slices[(i + 1) % wall_slices.size()];
      for (size_t j = 0; j < slice.size(); ++j) {
        shapes.push_back(Hull(slice[j], next_slice[j]));
        // Uncomment for testing. Much faster and easier to visualize.
        // shapes.push_back(slice[j]);
      }
    }
  }

  for (Key* key : d.all_keys()) {
    shapes.push_back(key->GetSwitch());
    if (kAddCaps) {
      shapes.push_back(key->GetCap().Color("red"));
    }
  }

  // Add all the screw inserts.
  std::vector<Shape> screw_holes;
  std::vector<Shape> screw_holes_bottom;
  {
    double screw_height = 6;
    double screw_radius = 3.5 / 2.0;
    double screw_insert_radius = 4.3 / 2.0;
    Shape screw_hole = Cylinder(screw_height + 12, screw_insert_radius, 40);
    Shape screw_hole_bottom = Cylinder(screw_height + 12, screw_radius, 40);
    Shape screw_insert =
        Cylinder(screw_height, screw_insert_radius + 3, 30).TranslateZ(screw_height / 2);

    glm::vec3 screw_left_bottom = d.key_fn.GetBottomLeft().Apply(kOrigin);
    screw_left_bottom.z = 0;
    screw_left_bottom.x += 3;
    screw_left_bottom.y += 0;

    glm::vec3 screw_left_top = d.key_plus.GetTopLeft().Apply(kOrigin);
    screw_left_top.z = 0;
    screw_left_top.x += 2.8;
    screw_left_top.y += -2;

    glm::vec3 screw_right_top = d.key_m1.GetTopRight().Apply(kOrigin);
    screw_right_top.z = 0;
    screw_right_top.x += -1;
    screw_right_top.y += -0.5;

    glm::vec3 screw_right_bottom = d.key_th1.GetBottomLeft().Apply(kOrigin);
    screw_right_bottom.z = 0;
    screw_right_bottom.y += 4;
    screw_right_bottom.x += 0;

    glm::vec3 screw_right_mid = d.key_th_top3.GetTopRight().Apply(kOrigin);
    screw_right_mid.z = 0;
    screw_right_mid.x += -6;
    screw_right_mid.y += 2;

    glm::vec3 screw_right_mid1 = d.key_th_bottom2.GetBottomRight().Apply(kOrigin);
    screw_right_mid1.z = 0;
    screw_right_mid1.x += -3;
    screw_right_mid1.y += 6;

    glm::vec3 screw_right_mid2 = d.key_th3.GetBottomRight().Apply(kOrigin);
    screw_right_mid2.z = 0;
    screw_right_mid2.x += -5;
    screw_right_mid2.y += 4;

    glm::vec3 screw_right_mid3 = d.key_3.GetTopRight().Apply(kOrigin);
    screw_right_mid3.z = 0;
    screw_right_mid3.x += 0;
    screw_right_mid3.y += 0;

    shapes.push_back(Union(screw_insert.Translate(screw_left_top),
                           screw_insert.Translate(screw_right_top),
                           screw_insert.Translate(screw_right_mid),
                           screw_insert.Translate(screw_right_mid1),
                           screw_insert.Translate(screw_right_mid2),
                           screw_insert.Translate(screw_right_mid3),
                           screw_insert.Translate(screw_right_bottom),
                           screw_insert.Translate(screw_left_bottom)));
    screw_holes = {
        screw_hole.Translate(screw_left_top),
        screw_hole.Translate(screw_right_top),
        screw_hole.Translate(screw_right_mid),
        screw_hole.Translate(screw_right_mid1),
        screw_hole.Translate(screw_right_mid2),
        screw_hole.Translate(screw_right_mid3),
        screw_hole.Translate(screw_right_bottom),
        screw_hole.Translate(screw_left_bottom),
    };
    screw_holes_bottom = {
        screw_hole_bottom.Translate(screw_left_top),
        screw_hole_bottom.Translate(screw_right_top),
        screw_hole_bottom.Translate(screw_right_mid),
        screw_hole_bottom.Translate(screw_right_mid1),
        screw_hole_bottom.Translate(screw_right_mid2),
        screw_hole_bottom.Translate(screw_right_mid3),
        screw_hole_bottom.Translate(screw_right_bottom),
        screw_hole_bottom.Translate(screw_left_bottom),
    };
  }


  std::vector<Shape> negative_shapes;

  Key b_cut = d.key_b;
  b_cut.Configure([&](Key& k) {
    k.AddTransform();
    k.t().z = -4;
    k.add_side_nub = false;
    // k.disable_switch_z_offset = true;
  });
  negative_shapes.push_back(b_cut.GetInverseSwitch());
  AddShapes(&negative_shapes, screw_holes);
  // Cut out hole for holder.
  Shape holder_hole = Cube(29.0, 20.0, 12.5).TranslateZ(12 / 2);
  glm::vec3 holder_location = d.key_5.GetTopLeft().Apply(kOrigin);
  holder_location.z = -0.5;
  holder_location.x += 12.5;//17.5
  negative_shapes.push_back(holder_hole.Translate(holder_location));

  Shape result = UnionAll(shapes);
  // Subtracting is expensive to preview and is best to disable while testing.
  result = result.Subtract(UnionAll(negative_shapes));
  result.WriteToFile("left.scad");
  result.MirrorX().WriteToFile("right.scad");

  // Bottom plate
  {
    std::vector<Shape> bottom_plate_shapes = {result};
    for (Key* key : d.all_keys()) {
      bottom_plate_shapes.push_back(Hull(key->GetSwitch()));
    }

    Shape bottom_plate = UnionAll(bottom_plate_shapes)
                             .Projection()
                             .LinearExtrude(3)
                             .Subtract(UnionAll(screw_holes_bottom));

    bottom_plate.WriteToFile("bottom_left.scad");
    bottom_plate.MirrorX().WriteToFile("bottom_right.scad");
  }

  return 0;
}

Shape ConnectMainKeys(KeyData& d) {
  std::vector<Shape> shapes;
  for (int r = 0; r < d.grid.num_rows(); ++r) {
    for (int c = 0; c < d.grid.num_columns(); ++c) {
      Key* key = d.grid.get_key(r, c);
      if (!key) {
        // No key at this location.
        continue;
      }
      Key* left = d.grid.get_key(r, c - 1);
      Key* top_left = d.grid.get_key(r - 1, c - 1);
      Key* top = d.grid.get_key(r - 1, c);

      if (left) {
        shapes.push_back(ConnectHorizontal(*left, *key));
      }
      if (top) {
        shapes.push_back(ConnectVertical(*top, *key));
        if (left && top_left) {
          shapes.push_back(ConnectDiagonal(*top_left, *top, *key, *left));
        }
      }
    }
  }
  return UnionAll(shapes);
}
