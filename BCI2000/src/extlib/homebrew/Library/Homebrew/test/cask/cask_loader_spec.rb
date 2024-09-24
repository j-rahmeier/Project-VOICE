# frozen_string_literal: true

RSpec.describe Cask::CaskLoader, :cask do
  describe "::for" do
    let(:tap) { CoreCaskTap.instance }

    context "when a cask is renamed" do
      let(:old_token) { "version-newest" }
      let(:new_token) { "version-latest" }

      let(:api_casks) do
        [old_token, new_token].to_h do |token|
          hash = described_class.load(new_token).to_hash_with_variations
          json = JSON.pretty_generate(hash)
          cask_json = JSON.parse(json)

          [token, cask_json.except("token")]
        end
      end
      let(:cask_renames) do
        { old_token => new_token }
      end

      before do
        allow(Homebrew::API::Cask)
          .to receive(:all_casks)
          .and_return(api_casks)

        allow(tap).to receive(:cask_renames)
          .and_return(cask_renames)
      end

      context "when not using the API", :no_api do
        it "warns when using the short token" do
          expect do
            expect(described_class.for("version-newest")).to be_a Cask::CaskLoader::FromPathLoader
          end.to output(/version-newest was renamed to version-latest/).to_stderr
        end

        it "warns when using the full token" do
          expect do
            expect(described_class.for("homebrew/cask/version-newest")).to be_a Cask::CaskLoader::FromPathLoader
          end.to output(/version-newest was renamed to version-latest/).to_stderr
        end
      end

      context "when using the API" do
        before do
          ENV.delete("HOMEBREW_NO_INSTALL_FROM_API")
        end

        it "warns when using the short token" do
          expect do
            expect(described_class.for("version-newest")).to be_a Cask::CaskLoader::FromAPILoader
          end.to output(/version-newest was renamed to version-latest/).to_stderr
        end

        it "warns when using the full token" do
          expect do
            expect(described_class.for("homebrew/cask/version-newest")).to be_a Cask::CaskLoader::FromAPILoader
          end.to output(/version-newest was renamed to version-latest/).to_stderr
        end
      end
    end

    context "when not using the API", :no_api do
      context "when a cask is migrated" do
        let(:token) { "local-caffeine" }

        let(:core_tap) { CoreTap.instance }
        let(:core_cask_tap) { CoreCaskTap.instance }

        let(:tap_migrations) do
          {
            token => new_tap.name,
          }
        end

        before do
          old_tap.path.mkpath
          new_tap.path.mkpath
          (old_tap.path/"tap_migrations.json").write tap_migrations.to_json
        end

        context "to a cask in an other tap" do
          # Can't use local-caffeine. It is a fixture in the :core_cask_tap and would take precendence over :new_tap.
          let(:token) { "some-cask" }

          let(:old_tap) { Tap.fetch("homebrew", "foo") }
          let(:new_tap) { Tap.fetch("homebrew", "bar") }

          let(:cask_file) { new_tap.cask_dir/"#{token}.rb" }

          before do
            new_tap.cask_dir.mkpath
            FileUtils.touch cask_file
          end

          # FIXME
          # It would be preferable not to print a warning when installing with the short token
          it "warns when loading the short token" do
            expect do
              described_class.for(token)
            end.to output(%r{Cask #{old_tap}/#{token} was renamed to #{new_tap}/#{token}\.}).to_stderr
          end

          it "does not warn when loading the full token in the new tap" do
            expect do
              described_class.for("#{new_tap}/#{token}")
            end.not_to output.to_stderr
          end

          it "warns when loading the full token in the old tap" do
            expect do
              described_class.for("#{old_tap}/#{token}")
            end.to output(%r{Cask #{old_tap}/#{token} was renamed to #{new_tap}/#{token}\.}).to_stderr
          end
        end

        context "to a formula in the default tap" do
          let(:old_tap) { core_cask_tap }
          let(:new_tap) { core_tap }

          let(:formula_file) { new_tap.formula_dir/"#{token}.rb" }

          before do
            new_tap.formula_dir.mkpath
            FileUtils.touch formula_file
          end

          it "warn only once" do
            expect do
              described_class.for(token)
            end.to output(
              a_string_including("Warning: Cask #{token} was renamed to #{new_tap}/#{token}.").once,
            ).to_stderr
          end
        end

        context "to the default tap" do
          let(:old_tap) { core_tap }
          let(:new_tap) { core_cask_tap }

          let(:cask_file) { new_tap.cask_dir/"#{token}.rb" }

          before do
            new_tap.cask_dir.mkpath
            FileUtils.touch cask_file
          end

          it "does not warn when loading the short token" do
            expect do
              described_class.for(token)
            end.not_to output.to_stderr
          end

          it "does not warn when loading the full token in the default tap" do
            expect do
              described_class.for("#{new_tap}/#{token}")
            end.not_to output.to_stderr
          end

          it "warns when loading the full token in the old tap" do
            expect do
              described_class.for("#{old_tap}/#{token}")
            end.to output(%r{Cask #{old_tap}/#{token} was renamed to #{token}\.}).to_stderr
          end

          # FIXME
          # context "when there is an infinite tap migration loop" do
          #   before do
          #     (new_tap.path/"tap_migrations.json").write({
          #       token => old_tap.name,
          #     }.to_json)
          #   end
          #
          #   it "stops recursing" do
          #     expect do
          #       described_class.for("#{new_tap}/#{token}")
          #     end.not_to output.to_stderr
          #   end
          # end
        end
      end
    end
  end

  describe "::load_prefer_installed" do
    let(:foo_tap) { Tap.fetch("user", "foo") }
    let(:bar_tap) { Tap.fetch("user", "bar") }

    let(:blank_tab) { instance_double(Cask::Tab, tap: nil) }
    let(:installed_tab) { instance_double(Cask::Tab, tap: bar_tap) }

    let(:cask_with_foo_tap) { instance_double(Cask::Cask, token: "test-cask", tap: foo_tap) }
    let(:cask_with_bar_tap) { instance_double(Cask::Cask, token: "test-cask", tap: bar_tap) }

    let(:load_args) { { config: nil, warn: true } }

    before do
      allow(described_class).to receive(:load).with("test-cask", load_args).and_return(cask_with_foo_tap)
      allow(described_class).to receive(:load).with("user/foo/test-cask", load_args).and_return(cask_with_foo_tap)
      allow(described_class).to receive(:load).with("user/bar/test-cask", load_args).and_return(cask_with_bar_tap)
    end

    it "returns the correct cask when no tap is specified and no tab exists" do
      allow_any_instance_of(Cask::Cask).to receive(:tab).and_return(blank_tab)
      expect(described_class).to receive(:load).with("test-cask", load_args)

      expect(described_class.load_prefer_installed("test-cask").tap).to eq(foo_tap)
    end

    it "returns the correct cask when no tap is specified but a tab exists" do
      allow_any_instance_of(Cask::Cask).to receive(:tab).and_return(installed_tab)
      expect(described_class).to receive(:load).with("user/bar/test-cask", load_args)

      expect(described_class.load_prefer_installed("test-cask").tap).to eq(bar_tap)
    end

    it "returns the correct cask when a tap is specified and no tab exists" do
      allow_any_instance_of(Cask::Cask).to receive(:tab).and_return(blank_tab)
      expect(described_class).to receive(:load).with("user/bar/test-cask", load_args)

      expect(described_class.load_prefer_installed("user/bar/test-cask").tap).to eq(bar_tap)
    end

    it "returns the correct cask when no tap is specified and a tab exists" do
      allow_any_instance_of(Cask::Cask).to receive(:tab).and_return(installed_tab)
      expect(described_class).to receive(:load).with("user/foo/test-cask", load_args)

      expect(described_class.load_prefer_installed("user/foo/test-cask").tap).to eq(foo_tap)
    end

    it "returns the correct cask when no tap is specified and the tab lists an tap that isn't installed" do
      allow_any_instance_of(Cask::Cask).to receive(:tab).and_return(installed_tab)
      expect(described_class).to receive(:load).with("user/bar/test-cask", load_args)
                                               .and_raise(Cask::CaskUnavailableError.new("test-cask", bar_tap))
      expect(described_class).to receive(:load).with("test-cask", load_args)

      expect(described_class.load_prefer_installed("test-cask").tap).to eq(foo_tap)
    end
  end
end
