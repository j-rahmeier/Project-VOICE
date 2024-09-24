# typed: true # rubocop:todo Sorbet/StrictSigil
# frozen_string_literal: true

require "url"
require "checksum"
require "download_strategy"

module Downloadable
  include Context
  extend T::Helpers

  abstract!

  sig { overridable.returns(T.nilable(URL)) }
  attr_reader :url

  sig { overridable.returns(T.nilable(Checksum)) }
  attr_reader :checksum

  sig { overridable.returns(T::Array[String]) }
  attr_reader :mirrors

  sig { void }
  def initialize
    @mirrors = T.let([], T::Array[String])
  end

  def initialize_dup(other)
    super
    @checksum = @checksum.dup
    @mirrors = @mirrors.dup
    @version = @version.dup
  end

  sig { overridable.returns(T.self_type) }
  def freeze
    @checksum.freeze
    @mirrors.freeze
    @version.freeze
    super
  end

  sig { abstract.returns(String) }
  def name; end

  sig { returns(String) }
  def download_type
    T.must(self.class.name&.split("::")&.last).gsub(/([[:lower:]])([[:upper:]])/, '\1 \2').downcase
  end

  sig(:final) { returns(T::Boolean) }
  def downloaded?
    cached_download.exist?
  end

  sig { overridable.returns(Pathname) }
  def cached_download
    downloader.cached_location
  end

  sig { overridable.void }
  def clear_cache
    downloader.clear_cache
  end

  sig { overridable.returns(T.nilable(Version)) }
  def version
    return @version if @version && !@version.null?

    version = determine_url&.version
    version unless version&.null?
  end

  sig { overridable.returns(T.class_of(AbstractDownloadStrategy)) }
  def download_strategy
    @download_strategy ||= determine_url&.download_strategy
  end

  sig { overridable.returns(AbstractDownloadStrategy) }
  def downloader
    @downloader ||= begin
      primary_url, *mirrors = determine_url_mirrors
      raise ArgumentError, "attempted to use a `Downloadable` without a URL!" if primary_url.blank?

      download_strategy.new(primary_url, download_name, version,
                            mirrors:, cache:, **T.must(@url).specs)
    end
  end

  sig {
    overridable.params(
      verify_download_integrity: T::Boolean,
      timeout:                   T.nilable(T.any(Integer, Float)),
      quiet:                     T::Boolean,
    ).returns(Pathname)
  }
  def fetch(verify_download_integrity: true, timeout: nil, quiet: false)
    cache.mkpath

    begin
      downloader.quiet! if quiet
      downloader.fetch(timeout:)
    rescue ErrorDuringExecution, CurlDownloadStrategyError => e
      raise DownloadError.new(self, e)
    end

    download = cached_download
    verify_download_integrity(download) if verify_download_integrity
    download
  end

  sig { overridable.params(filename: Pathname).void }
  def verify_download_integrity(filename)
    if filename.file?
      ohai "Verifying checksum for '#{filename.basename}'" if verbose?
      filename.verify_checksum(checksum)
    end
  rescue ChecksumMissingError
    opoo <<~EOS
      Cannot verify integrity of '#{filename.basename}'.
      No checksum was provided.
      For your reference, the checksum is:
        sha256 "#{filename.sha256}"
    EOS
  end

  sig { overridable.returns(String) }
  def download_name
    @download_name ||= File.basename(determine_url.to_s)
  end

  private

  sig { overridable.returns(T.nilable(URL)) }
  def determine_url
    @url
  end

  sig { overridable.returns(T::Array[String]) }
  def determine_url_mirrors
    [determine_url.to_s, *mirrors].uniq
  end

  sig { overridable.returns(Pathname) }
  def cache
    HOMEBREW_CACHE
  end
end
